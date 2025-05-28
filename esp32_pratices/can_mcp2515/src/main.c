#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/can.h>

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(led_0), gpios);
static const struct device *can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));

void rx_callback_function(const struct device *dev, struct can_frame *frame, void *user_data)
{
    printk("\nFRAME ID: [%d]", frame->id);
    printk("FRAME DLC: [%d]", frame->dlc);
    for (int i = 0; i < frame->dlc; i++)
    {
        printk("Frame[%d]: %d ", i, frame->data[i]);
    }
}

struct can_frame can_frame = {
    .flags = 0,
    .id = 0x123,
    .dlc = 8,
    .data = {1, 2, 3, 4, 5, 6, 7, 8}};

const struct can_filter my_filter = {
    .flags = 0,
    .id = 0x123,
    .mask = 0};

void send_can_data()
{
    int ret = can_send(can_dev, &can_frame, K_MSEC(100), NULL, NULL);
    if (ret != 0)
    {
        printk("Sending failed [%d]", ret);
    }
}

void main(void)
{
    int ret = 0;
    int filter;
    // check can device

    __ASSERT(device_is_ready(can_dev) == true, "can device not ready");
    __ASSERT(can_set_mode(can_dev, CAN_MODE_LOOPBACK) == 0, "can_start() returned != zero!");
    filter = can_add_rx_filter(can_dev, rx_callback_function, NULL, &my_filter);
    __ASSERT(can_start(can_dev) == 0, "can_start() returned != zero!");
    
    printk("CAN OK\n");

    if (!device_is_ready(led.port))
    {
        return;
    }
    else
    {
        ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
        if (ret < 0)
        {
            return;
        }
        else
        {
            // just to dont leave a if withou else ;p
        }
    }

    while (1)
    {
        // printk("Hello, Zephyr!\n");
        ret = gpio_pin_toggle_dt(&led);
        send_can_data();
        k_sleep(K_MSEC(500));
    }
}
