#include "shop_common.h"
#include <fcntl.h>
#include <sys/stat.h>

static struct shared_inventory *inventory = NULL;
static int shm_id = -1;
static int sem_id = -1;
static int msg_id = -1;
static volatile sig_atomic_t running = 1;

/* Signal handler for graceful shutdown */
static void signal_handler(int signo __attribute__((unused))) {
    running = 0;
}

/* Initialize semaphore */
static void init_semaphore(void) {
    key_t sem_key = get_key('B');
    union semun sem_union;

    sem_id = semget(sem_key, 1, IPC_CREAT | 0666);
    CHECK(sem_id != -1);

    sem_union.val = 1;
    CHECK(semctl(sem_id, 0, SETVAL, sem_union) != -1);
}

/* Semaphore operations */
static void sem_lock(void) {
    struct sembuf sb = {0, -1, SEM_UNDO};
    CHECK(semop(sem_id, &sb, 1) != -1);
}

static void sem_unlock(void) {
    struct sembuf sb = {0, 1, SEM_UNDO};
    CHECK(semop(sem_id, &sb, 1) != -1);
}

/* Initialize shared memory */
static void init_shared_memory(void) {
    key_t shm_key = get_key('A');
    
    shm_id = shmget(shm_key, sizeof(struct shared_inventory), IPC_CREAT | 0666);
    CHECK(shm_id != -1);

    inventory = (struct shared_inventory *)shmat(shm_id, NULL, 0);
    CHECK(inventory != (void *)-1);
    
    memset(inventory, 0, sizeof(struct shared_inventory));
}

/* Initialize message queue */
static void init_message_queue(void) {
    key_t msg_key = get_key('C');
    
    msg_id = msgget(msg_key, IPC_CREAT | 0666);
    CHECK(msg_id != -1);
}

/* Load initial inventory from file */
static void load_inventory(void) {
    FILE *fp = fopen(INVENTORY_FILE, "r");
    CHECK(fp != NULL);

    char name[MAX_ITEM_NAME];
    double price;
    int quantity, threshold;
    int idx = 0;

    while (fscanf(fp, "%49s %lf %d %d", name, &price, &quantity, &threshold) == 4) {
        safe_strncpy(inventory->items[idx].name, name, MAX_ITEM_NAME);
        inventory->items[idx].price = price;
        inventory->items[idx].quantity = quantity;
        inventory->items[idx].low_threshold = threshold;
        idx++;
    }

    inventory->item_count = idx;
    fclose(fp);
}

/* Display current inventory */
static void display_inventory(void) {
    time_t now;
    struct tm *tm_info;
    char time_str[26];

    time(&now);
    tm_info = localtime(&now);
    strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    printf("\n[%s]\n", time_str);
    printf("\033[36m=== Current Inventory Status ===\033[0m\n");
    printf("%-20s %-10s %-10s\n", "Item", "Quantity", "Price");
    printf("--------------------------------\n");

    for (int i = 0; i < inventory->item_count; i++) {
        printf("%-20s %-10d $%.2f\n",
               inventory->items[i].name,
               inventory->items[i].quantity,
               inventory->items[i].price);
    }
    printf("\n");
}

/* Send inventory notification */
static void send_notification(const char *item_name, int quantity, int threshold, int notify_type) {
    struct notify_msg msg;
    memset(&msg, 0, sizeof(struct notify_msg));
    
    msg.mtype = (notify_type == NOTIFY_LOW_STOCK) ? MSG_NOTIFY_LOW_STOCK : MSG_NOTIFY_OUT_OF_STOCK;
    safe_strncpy(msg.data.item_name, item_name, MAX_ITEM_NAME);
    msg.data.current_quantity = quantity;
    msg.data.threshold = threshold;
    msg.data.notify_type = notify_type;
    
    if (msgsnd(msg_id, &msg, sizeof(msg.data), IPC_NOWAIT) == -1) {
        if (errno != EAGAIN) {  // Ignore if message queue is full
            perror("Failed to send notification");
        }
    }
}

/* Process customer order */
static void process_order(struct order_msg *msg) {
    msg->data.status = 0;  // Default to failure

    sem_lock();
    
    for (int i = 0; i < inventory->item_count; i++) {
        if (strcmp(inventory->items[i].name, msg->data.item_name) == 0) {
            if (inventory->items[i].quantity >= msg->data.quantity) {
                int new_quantity = inventory->items[i].quantity - msg->data.quantity;
                inventory->items[i].quantity = new_quantity;
                msg->data.status = 1;  // Success
                msg->data.price = inventory->items[i].price * msg->data.quantity;
                
                // Check if restock is needed
                if (new_quantity == 0) {
                    send_notification(inventory->items[i].name, 
                                   new_quantity,
                                   inventory->items[i].low_threshold,
                                   NOTIFY_OUT_OF_STOCK);
                } else if (new_quantity <= inventory->items[i].low_threshold) {
                    send_notification(inventory->items[i].name,
                                   new_quantity,
                                   inventory->items[i].low_threshold,
                                   NOTIFY_LOW_STOCK);
                }
            }
            break;
        }
    }
    
    sem_unlock();

    // Send response
    msg->mtype = MSG_RESPONSE;
    CHECK(msgsnd(msg_id, msg, sizeof(msg->data), 0) != -1);
}

int main(void) {
    struct sigaction sa;
    struct order_msg msg;

    // Set up signal handler
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    // Initialize IPC mechanisms
    init_semaphore();
    init_shared_memory();
    init_message_queue();
    
    // Load initial inventory
    load_inventory();

    printf("Shop process started. Press Ctrl+C to exit.\n");

    while (running) {
        // Display inventory periodically
        display_inventory();

        // Check for orders
        if (msgrcv(msg_id, &msg, sizeof(msg.data), MSG_ORDER, IPC_NOWAIT) != -1) {
            process_order(&msg);
        }

        sleep(2);  // Update interval
    }

    // Cleanup
    shmdt(inventory);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    msgctl(msg_id, IPC_RMID, NULL);

    return 0;
} 