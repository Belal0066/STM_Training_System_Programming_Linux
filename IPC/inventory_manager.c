#include "shop_common.h"
#include <signal.h>
#include <pthread.h>
#include <unistd.h> 

static struct shared_inventory *inventory = NULL;
static int shm_id = -1;
static int sem_id = -1;
static int msg_id = -1;
static volatile sig_atomic_t running = 1;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* Signal handler for graceful shutdown */
static void signal_handler(int signo __attribute__((unused))) {
    running = 0;
}

/* Initialize shared memory */
static void init_shared_memory(void) {
    key_t shm_key = get_key('A');
    shm_id = shmget(shm_key, sizeof(struct shared_inventory), 0666);
    CHECK(shm_id != -1);
    inventory = (struct shared_inventory *)shmat(shm_id, NULL, 0);
    CHECK(inventory != (void *)-1);
}

/* Initialize semaphore */
static void init_semaphore(void) {
    key_t sem_key = get_key('B');
    sem_id = semget(sem_key, 1, 0666);
    CHECK(sem_id != -1);
}

/* Initialize message queue */
static void init_message_queue(void) {
    key_t msg_key = get_key('C');
    msg_id = msgget(msg_key, 0666);
    CHECK(msg_id != -1);
}

/* Semaphore operations */
static void sem_lock(void) {
    struct sembuf sb = {0, -1, SEM_UNDO};
    while (1) {
        if (semop(sem_id, &sb, 1) == -1) {
            if (errno == EINTR && running) {
                continue;
            }
            perror("Failed to lock semaphore");
            return;
        }
        break;
    }
}

static void sem_unlock(void) {
    struct sembuf sb = {0, 1, SEM_UNDO};
    while (1) {
        if (semop(sem_id, &sb, 1) == -1) {
            if (errno == EINTR && running) {
                continue;
            }
            perror("Failed to unlock semaphore");
            return;
        }
        break;
    }
}

/* Display current inventory */
static void display_inventory(void) {
    sem_lock();
    printf("\n=== Current Inventory Status ===\n");
    printf("%-20s %-10s %-10s %-10s\n", "Item", "Quantity", "Price", "Threshold");
    printf("-------------------------------------------\n");

    for (int i = 0; i < inventory->item_count; i++) {
        printf("%-20s %-10d $%-9.2f %-10d\n",
               inventory->items[i].name,
               inventory->items[i].quantity,
               inventory->items[i].price,
               inventory->items[i].low_threshold);
    }
    printf("\n");
    sem_unlock();
}

/* Update item quantity */
static void restock_item(void) {
    char item_name[MAX_ITEM_NAME];
    int quantity;
    int found = 0;

    display_inventory();

    printf("Enter item name to restock: ");
    if (scanf("%49s", item_name) != 1) {
        printf("Invalid input\n");
        return;
    }

    printf("Enter quantity to add: ");
    if (scanf("%d", &quantity) != 1 || quantity <= 0) {
        printf("Invalid quantity\n");
        return;
    }

    sem_lock();
    for (int i = 0; i < inventory->item_count; i++) {
        if (strcmp(inventory->items[i].name, item_name) == 0) {
            inventory->items[i].quantity += quantity;
            printf("Successfully restocked %d units of %s\n", quantity, item_name);
            found = 1;
            break;
        }
    }
    sem_unlock();

    if (!found) {
        printf("Item not found\n");
    }
}

/* Update item price */
static void update_price(void) {
    char item_name[MAX_ITEM_NAME];
    double new_price;
    int found = 0;

    display_inventory();

    printf("Enter item name to update price: ");
    if (scanf("%49s", item_name) != 1) {
        printf("Invalid input\n");
        return;
    }

    printf("Enter new price: ");
    if (scanf("%lf", &new_price) != 1 || new_price <= 0) {
        printf("Invalid price\n");
        return;
    }

    sem_lock();
    for (int i = 0; i < inventory->item_count; i++) {
        if (strcmp(inventory->items[i].name, item_name) == 0) {
            inventory->items[i].price = new_price;
            printf("Successfully updated price of %s to $%.2f\n", item_name, new_price);
            found = 1;
            break;
        }
    }
    sem_unlock();

    if (!found) {
        printf("Item not found\n");
    }
}

/* Process notifications */
static void process_notification(const struct notify_msg *msg) {
    pthread_mutex_lock(&mutex);
    printf("\r\033[K");  // Clear current line

    if (msg->mtype == MSG_NOTIFY_LOW_STOCK) {
        printf("\n\033[33m*** ALERT: Low Stock ***\033[0m\n");
        printf("Item: %s\n", msg->data.item_name);
        printf("Current quantity: %d\n", msg->data.current_quantity);
        printf("Threshold: %d\n", msg->data.threshold);
        printf("Action needed: Consider restocking\n\n");
    } else if (msg->mtype == MSG_NOTIFY_OUT_OF_STOCK) {
        printf("\n\033[31m!!! URGENT: Out of Stock !!!\033[0m\n");
        printf("Item: %s\n", msg->data.item_name);
        printf("Current quantity: %d\n", msg->data.current_quantity);
        printf("Threshold: %d\n", msg->data.threshold);
        printf("Action needed: Immediate restock required\n\n");
    }

    printf("=== Inventory Manager Menu ===\n");
    printf("1. View inventory\n");
    printf("2. Restock item\n");
    printf("3. Update price\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
    fflush(stdout);
    pthread_mutex_unlock(&mutex);
}

/* Notification thread function */
static void *notification_handler(void *arg __attribute__((unused))) {
    struct notify_msg msg;
    
    while (running) {
        // Wait for notifications with a timeout
        struct timespec timeout;
        clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_sec += 1;  // 1 second timeout

        if (msgrcv(msg_id, &msg, sizeof(msg.data), 
                  MSG_NOTIFY_LOW_STOCK, IPC_NOWAIT) != -1) {
            process_notification(&msg);
        } else if (msgrcv(msg_id, &msg, sizeof(msg.data), 
                         MSG_NOTIFY_OUT_OF_STOCK, IPC_NOWAIT) != -1) {
            process_notification(&msg);
        } else if (errno != ENOMSG) {
            // Only sleep on real errors, not just empty queue
            sleep(1);
        }

        // Small sleep to prevent tight loop
        struct timespec req = {0, 100000000};  // 100ms
        nanosleep(&req, NULL);
    }
    return NULL;
}

/* Display menu */
static void display_menu(void) {
    pthread_mutex_lock(&mutex);
    printf("\033[36m=== Inventory Manager Menu ===\033[0m\n");
    printf("1. View inventory\n");
    printf("2. Restock item\n");
    printf("3. Update price\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
    fflush(stdout);
    pthread_mutex_unlock(&mutex);
}

int main(void) {
    pthread_t notifier;
    struct sigaction sa;

    // Set up signal handler
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    // Initialize IPC mechanisms
    init_shared_memory();
    init_semaphore();
    init_message_queue();

    // Create notification thread
    if (pthread_create(&notifier, NULL, notification_handler, NULL) != 0) {
        perror("Failed to create notification thread");
        goto cleanup;
    }

    printf("Inventory manager process started.\n");

    int choice;
    while (running) {
        display_menu();
        
        if (scanf("%d", &choice) != 1) {
            if (!running) break;
            printf("Invalid input\n");
            while (getchar() != '\n' && running);  // Clear input buffer
            continue;
        }

        if (!running) break;

        switch (choice) {
            case 1:
                display_inventory();
                break;
            case 2:
                restock_item();
                break;
            case 3:
                update_price();
                break;
            case 4:
                running = 0;
                break;
            default:
                printf("Invalid choice\n");
        }
    }

cleanup:
    running = 0;
    pthread_join(notifier, NULL);
    pthread_mutex_destroy(&mutex);
    shmdt(inventory);
    return 0;
} 