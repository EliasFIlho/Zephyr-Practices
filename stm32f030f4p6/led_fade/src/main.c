#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/device.h>  

static const struct pwm_dt_spec led = PWM_DT_SPEC_GET(DT_NODELABEL(fade_led));

void main(void){
    if(!device_is_ready(led.dev)){
        printk("Error: PWM device %s is not ready\n",led.dev->name);
        return;
    }
    int ret;
    printk("STARTING LED FADE");
    while(1){
        for(int i = 0;i<10000;i++){
            ret = pwm_set_pulse_dt(&led,i);
            printk("LED STATUS: %d\n",i);
            k_usleep(10);
        }
        k_msleep(300);

        for(int i = 10000;i>0;i--){
            ret = pwm_set_pulse_dt(&led,i);
            printk("LED STATUS: %d\n",i);
            k_usleep(10);
        }
        k_msleep(300);
    }
}