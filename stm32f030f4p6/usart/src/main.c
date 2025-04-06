#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(led_0),gpios);

#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)
static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

int main(void) {
    int ret = 0;
    if(!device_is_ready(led.port)){
        return;
    }else{
        ret = gpio_pin_configure_dt(&led,GPIO_OUTPUT);  
        if(ret < 0){
            return 0;
        }else{
            // just to dont leave a if withou else ;p            
        }
    }

    if (!device_is_ready(uart_dev)) {
	    printk("UART device not found!");
		return 0;
	}

    while (1) {
        gpio_pin_toggle_dt(&led);
        uart_poll_out(uart_dev,'A');
        k_sleep(K_MSEC(100));

    }

    return 0;
}
