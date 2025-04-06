#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(led_0),gpios);
static const struct adc_dt_spec adc_ch0 =  ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0);

#define LED_THRESHOLD 2048
#define HIGH 1
#define LOW 0
#define SAMPLE_NUM 100

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

     if(!device_is_ready(adc_ch0.dev)){
        return;
    }else{
        ret = adc_channel_setup_dt(&adc_ch0);
		if (ret < 0) {
			//printk("Could not setup channel (%d)\n", ret);
			return 0;
		}else{
            //printk("ADC READY TO READ SAMPLES\n");
        }
    }

    int16_t sample;
    struct adc_sequence sequence = {
		.buffer = &sample,
		.buffer_size = sizeof(sample),
	};

    adc_sequence_init_dt(&adc_ch0,&sequence);
    adc_channel_setup_dt(&adc_ch0);
    while (1) {
        uint16_t avg = 0;
        k_sleep(K_USEC(100));
        for(uint8_t i = 0;i<SAMPLE_NUM;i++){
            ret = adc_read_dt(&adc_ch0,&sequence);  
            if(ret < 0){
                //printk("\nERROR TO READ DATA - (%d)",ret);
            }else{
                avg += ((sample - avg)/(i+1));
            }
        }
        printk("\nData - [%d]", avg);   
        if(avg <= LED_THRESHOLD){
            gpio_pin_set_dt(&led,HIGH);
        }else{
            gpio_pin_set_dt(&led,LOW);
        }
        k_sleep(K_MSEC(50));

    }
}
