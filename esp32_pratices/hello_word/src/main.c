#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

void main(void) {
    while (1) {
        printk("Hello, Zephyr!\n");
        k_sleep(K_SECONDS(1));

    }
}
