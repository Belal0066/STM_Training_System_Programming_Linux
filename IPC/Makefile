CC = gcc

TARGETS = shop_process customer_process inventory_manager

all: $(TARGETS)

shop_process: shop_process.c shop_common.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

customer_process: customer_process.c shop_common.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

inventory_manager: inventory_manager.c shop_common.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGETS)

.PHONY: all clean 