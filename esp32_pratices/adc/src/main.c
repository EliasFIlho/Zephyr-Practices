#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(led_0),gpios);
static const struct adc_dt_spec adc_ch0 =  ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0);


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
			printk("Could not setup channel (%d)\n", ret);
			return 0;
		}else{
            printk("ADC READY TO READ SAMPLES\n");
        }
    }

    uint16_t sample;
    struct adc_sequence sequence = {
		.buffer = &sample,
		.buffer_size = sizeof(sample),
	};

    adc_sequence_init_dt(&adc_ch0,&sequence);

    int32_t val;
    while (1) {
        adc_channel_setup_dt(&adc_ch0);
        k_sleep(K_USEC(100));
        gpio_pin_toggle_dt(&led);
        ret = adc_read_dt(&adc_ch0,&sequence);  
        if(ret < 0){
            printk("\nERROR TO READ DATA");
        }
        val = (int32_t)sample;
        printk("\nVALUE READED: %d", sample);

        if (adc_ch0.channel_cfg.differential) {
				val = (int32_t)((int16_t)sample);
		} else {
			val = (int32_t)sample;
		}
        k_sleep(K_MSEC(100));

    }
}
