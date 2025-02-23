#ifndef SHOP_COMMON_H
#define SHOP_COMMON_H

#define _POSIX_C_SOURCE 200809L 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#define MAX_ITEMS 100
#define MAX_ITEM_NAME 50
#define INVENTORY_FILE "inventory.txt"
#define PROJECT_ID 'S'

/* Message types */
#define MSG_ORDER 1
#define MSG_RESTOCK 2
#define MSG_PRICE_UPDATE 3
#define MSG_RESPONSE 4
#define MSG_NOTIFY_LOW_STOCK 5
#define MSG_NOTIFY_OUT_OF_STOCK 6

/* Notification types */
#define NOTIFY_LOW_STOCK 1
#define NOTIFY_OUT_OF_STOCK 2

/* Safe string copy function */
static inline void safe_strncpy(char *dest, const char *src, size_t n) {
    size_t len = strlen(src);
    if (len >= n) {
        len = n - 1;
    }
    memcpy(dest, src, len);
    dest[len] = '\0';
}

/* Shared memory structure for inventory items */
struct inventory_item {
    char name[MAX_ITEM_NAME];
    int quantity;
    double price;
    int low_threshold;  // For automatic restock notification
};

/* Shared memory structure */
struct shared_inventory {
    struct inventory_item items[MAX_ITEMS];
    int item_count;
    volatile sig_atomic_t updates_pending;
};

/* Message structure for orders */
struct order_msg {
    long mtype;
    struct {
        char item_name[MAX_ITEM_NAME];
        int quantity;
        double price;
        pid_t customer_pid;
        int status;  // 0 for failure, 1 for success
    } data;
};

/* Message structure for inventory notifications */
struct notify_msg {
    long mtype;
    struct {
        char item_name[MAX_ITEM_NAME];
        int current_quantity;
        int threshold;
        int notify_type;  // NOTIFY_LOW_STOCK or NOTIFY_OUT_OF_STOCK
    } data;
};

/* Semaphore operations */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

/* Key generation function */
static inline key_t get_key(char id) {
    return ftok(".", id);
}

/* Error checking macro */
#define CHECK(x) do { \
    if (!(x)) { \
        fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
        perror(#x); \
        exit(EXIT_FAILURE); \
    } \
} while (0)

#endif /* SHOP_COMMON_H */ 