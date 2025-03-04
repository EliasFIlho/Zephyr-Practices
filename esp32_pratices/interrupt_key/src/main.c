#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>  


void button_handler();

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(led_0),gpios);
static const struct gpio_dt_spec bt = GPIO_DT_SPEC_GET(DT_NODELABEL(button_0),gpios);
static struct gpio_callback button_cb_data;


void main(void) {
    int ret = 0;
    if(!device_is_ready(led.port)){
        return;
    }else{
        ret = gpio_pin_configure_dt(&led,GPIO_OUTPUT);  
        if(ret < 0){
            return;
        }
    }
    if(!device_is_ready(bt.port)){
        return;
    }else{
        ret = gpio_pin_configure_dt(&bt,GPIO_INPUT);  
        if(ret < 0){
            return;
        }
    }
    ret = gpio_pin_interrupt_configure_dt(&bt,GPIO_INT_EDGE_FALLING);
    gpio_init_callback(&button_cb_data,button_handler,BIT(bt.pin));
    gpio_add_callback(bt.port,&button_cb_data);

    while (1) {
        printk("Hello, Zephyr!\n");
        k_sleep(K_MSEC(1000));

    }
}
void button_handler(const struct device *dev, struct gpio_callback *cb,uint32_t pins){

	printk("Button pressed toggling led\n");
    gpio_pin_toggle_dt(&led);
    
}
