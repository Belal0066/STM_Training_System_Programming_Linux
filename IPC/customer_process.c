#include "shop_common.h"

#define MAX_CART_ITEMS 10

/* Shopping cart structure */
struct cart_item {
    char name[MAX_ITEM_NAME];
    int quantity;
    double unit_price;  // Store unit price for display
};

struct shopping_cart {
    struct cart_item items[MAX_CART_ITEMS];
    int item_count;
    double total_price;
};

static int msg_id = -1;
static struct shared_inventory *inventory = NULL;
static int shm_id = -1;
static struct shopping_cart cart = {0};

/* Initialize shared memory (read-only) */
static void init_shared_memory(void) {
    key_t shm_key = get_key('A');
    
    shm_id = shmget(shm_key, sizeof(struct shared_inventory), 0666);
    CHECK(shm_id != -1);

    inventory = (struct shared_inventory *)shmat(shm_id, NULL, SHM_RDONLY);
    CHECK(inventory != (void *)-1);
}

/* Initialize message queue */
static void init_message_queue(void) {
    key_t msg_key = get_key('C');
    
    msg_id = msgget(msg_key, 0666);
    CHECK(msg_id != -1);
}

/* Display available items */
static void display_available_items(void) {
    printf("\n=== Available Items ===\n");
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

/* Display shopping cart */
static void display_cart(void) {
    if (cart.item_count == 0) {
        printf("\nShopping cart is empty\n");
        return;
    }

    printf("\n=== Shopping Cart ===\n");
    printf("%-20s %-10s %-10s %-10s\n", "Item", "Quantity", "Unit Price", "Total");
    printf("---------------------------------------------------\n");

    for (int i = 0; i < cart.item_count; i++) {
        double item_total = cart.items[i].quantity * cart.items[i].unit_price;
        printf("%-20s %-10d $%-9.2f $%.2f\n",
               cart.items[i].name,
               cart.items[i].quantity,
               cart.items[i].unit_price,
               item_total);
    }
    printf("---------------------------------------------------\n");
    printf("Total: $%.2f\n\n", cart.total_price);
}

/* Add item to cart */
static void add_to_cart(void) {
    if (cart.item_count >= MAX_CART_ITEMS) {
        printf("Cart is full! Please checkout first.\n");
        return;
    }

    display_available_items();

    char item_name[MAX_ITEM_NAME];
    int quantity;

    printf("Enter item name: ");
    if (scanf("%49s", item_name) != 1) {
        printf("Invalid input\n");
        return;
    }

    printf("Enter quantity: ");
    if (scanf("%d", &quantity) != 1 || quantity <= 0) {
        printf("Invalid quantity\n");
        return;
    }

    // Check if item exists and has sufficient stock
    int found = 0;
    for (int i = 0; i < inventory->item_count; i++) {
        if (strcmp(inventory->items[i].name, item_name) == 0) {
            if (inventory->items[i].quantity >= quantity) {
                // Add to cart
                safe_strncpy(cart.items[cart.item_count].name, item_name, MAX_ITEM_NAME);
                cart.items[cart.item_count].quantity = quantity;
                cart.items[cart.item_count].unit_price = inventory->items[i].price;
                cart.total_price += quantity * inventory->items[i].price;
                cart.item_count++;
                printf("Added to cart: %d x %s\n", quantity, item_name);
                found = 1;
            } else {
                printf("Insufficient stock! Available: %d\n", inventory->items[i].quantity);
                found = 1;
            }
            break;
        }
    }

    if (!found) {
        printf("Item not found\n");
    }
}

/* Process checkout */
static void checkout(void) {
    if (cart.item_count == 0) {
        printf("Cart is empty!\n");
        return;
    }

    display_cart();
    printf("Proceed with checkout? (y/n): ");
    char confirm;
    while (getchar() != '\n');  // Clear input buffer
    if (scanf("%c", &confirm) != 1 || (confirm != 'y' && confirm != 'Y')) {
        printf("Checkout cancelled\n");
        return;
    }

    // Process each item in cart
    int all_success = 1;
    for (int i = 0; i < cart.item_count; i++) {
        struct order_msg msg;
        memset(&msg, 0, sizeof(struct order_msg));
        
        msg.mtype = MSG_ORDER;
        safe_strncpy(msg.data.item_name, cart.items[i].name, MAX_ITEM_NAME);
        msg.data.quantity = cart.items[i].quantity;
        msg.data.customer_pid = getpid();

        // Send order
        if (msgsnd(msg_id, &msg, sizeof(msg.data), 0) == -1) {
            perror("Failed to send message");
            all_success = 0;
            break;
        }

        // Wait for response
        if (msgrcv(msg_id, &msg, sizeof(msg.data), MSG_RESPONSE, 0) == -1) {
            perror("Failed to receive response");
            all_success = 0;
            break;
        }

        if (!msg.data.status) {
            printf("Failed to process item: %s\n", cart.items[i].name);
            all_success = 0;
            break;
        }
    }

    if (all_success) {
        printf("\nCheckout successful! Total paid: $%.2f\n", cart.total_price);
        // Clear cart
        memset(&cart, 0, sizeof(struct shopping_cart));
    } else {
        printf("\nCheckout failed! Please try again later.\n");
    }
}

/* Clear shopping cart */
static void clear_cart(void) {
    if (cart.item_count == 0) {
        printf("Cart is already empty\n");
        return;
    }

    printf("Are you sure you want to clear the cart? (y/n): ");
    char confirm;
    while (getchar() != '\n');  // Clear input buffer
    if (scanf("%c", &confirm) == 1 && (confirm == 'y' || confirm == 'Y')) {
        memset(&cart, 0, sizeof(struct shopping_cart));
        printf("Cart cleared\n");
    } else {
        printf("Operation cancelled\n");
    }
}

/* Display menu */
static void display_menu(void) {
    printf("\033[36m=== Customer Menu ===\033[0m\n");
    printf("1. View available items\n");
    printf("2. Add item to cart\n");
    printf("3. View cart\n");
    printf("4. Clear cart\n");
    printf("5. Checkout\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

int main(void) {
    int choice;

    // Initialize IPC mechanisms
    init_shared_memory();
    init_message_queue();

    printf("Customer process started.\n");

    while (1) {
        display_menu();
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');  // Clear input buffer
            continue;
        }

        switch (choice) {
            case 1:
                display_available_items();
                break;
            case 2:
                add_to_cart();
                break;
            case 3:
                display_cart();
                break;
            case 4:
                clear_cart();
                break;
            case 5:
                checkout();
                break;
            case 6:
                goto cleanup;
            default:
                printf("Invalid choice\n");
        }
    }

cleanup:
    shmdt(inventory);
    return 0;
} 