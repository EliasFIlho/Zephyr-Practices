#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/pwm.h>

    
static const struct pwm_dt_spec pwm = PWM_DT_SPEC_GET_BY_IDX(DT_NODELABEL(motor),0);

static const struct pwm_dt_spec out = PWM_DT_SPEC_GET_BY_IDX(DT_NODELABEL(motor),1);

#define PWM_PERIOD_NSEC 1000000
#define PWM_PULSE_NSEC   500000

#define PWM_POLARITY_INVERTED	(1 << 0)
#define PWM_POLARITY_NORMAL	(0 << 0)

#define FLAGS (PWM_CAPTURE_TYPE_PULSE|PWM_POLARITY_NORMAL)

int main(void) {
    if(device_is_ready(pwm.dev)){
        printk("Looks like he is ready!!\nDevice channel: {%d}\nDevice period: {%d}\n",pwm.channel,pwm.period);
    }else{
        printk("F is not ready yet\n");
    }

    if(device_is_ready(out.dev)){
        printk("Looks like out is ready!!\nDevice channel: {%d}\nDevice period: {%d}\n",out.channel,out.period);
    }else{
        printk("F out is not ready yet\n");
    }
    int err;
    err = pwm_set(pwm.dev, pwm.channel,PWM_PERIOD_NSEC ,PWM_PULSE_NSEC, PWM_POLARITY_NORMAL);
    if(err != 0){

        printk("Error to set pwm out: Err{%d}\n",err);
    }else{
        printk("PWM seted with a period of {%d} and pulse period of {%d}\n",PWM_PERIOD_NSEC,PWM_PULSE_NSEC);
    }
    while (1) {
        printk("FADE ON\n");
        for(int i = 0;i<PWM_PERIOD_NSEC;i++){
            pwm_set_cycles(pwm.dev, pwm.channel,PWM_PERIOD_NSEC ,i, PWM_POLARITY_NORMAL);
            printk("FADE ON STATUS [%d/%d]\n",i,PWM_PERIOD_NSEC);
            k_usleep(10);
        }

        k_sleep(K_SECONDS(1));
        printk("FADE OFF\n");
        for(int i = PWM_PERIOD_NSEC;i>0;i--){
            printk("FADE ON STATUS [%d/%d]\n",PWM_PERIOD_NSEC,i);
            pwm_set_cycles(pwm.dev, pwm.channel,PWM_PERIOD_NSEC ,i, PWM_POLARITY_NORMAL);
            k_usleep(10);
        }

        k_sleep(K_SECONDS(1));
    }
    return 0;
}
