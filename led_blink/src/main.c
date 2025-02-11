#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>  

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(led_0),gpios);


void main(void) {
    int ret = 0;
    if(!device_is_ready(led.port)){
        return;
    }else{
        ret = gpio_pin_configure_dt(&led,GPIO_OUTPUT);  
        if(ret < 0){
            return;
        }else{
            // just to dont leave a if withou else ;p            
        }
    }

    while (1) {
        printk("Hello, Zephyr!\n");
        ret = gpio_pin_toggle_dt(&led);
        k_sleep(K_MSEC(500));

    }
}
