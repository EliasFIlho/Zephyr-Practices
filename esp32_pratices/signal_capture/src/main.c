#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/pwm.h>

static const struct pwm_dt_spec pwm = PWM_DT_SPEC_GET_BY_IDX(DT_NODELABEL(motor), 0);

static const struct pwm_dt_spec cap = PWM_DT_SPEC_GET_BY_IDX(DT_NODELABEL(motor), 1);

#define PWM_PERIOD_NSEC 100000
#define PWM_PULSE_NSEC 2500

#define CLK_FREQUENCY 80000000

#define PWM_POLARITY_INVERTED (1 << 0)
#define PWM_POLARITY_NORMAL (0 << 0)

#define FLAGS (PWM_CAPTURE_TYPE_PULSE | PWM_POLARITY_NORMAL)

static void capture_handler(const struct device *dev, uint32_t channel, uint32_t period_cycles, uint32_t pulse_cycles, int status, void *user_data)
{
    if (status != 0)
    {
        printk("Error to capture - status: [%d]", status);
    }
    else
    {
        double frequency = 1.0 / ((double)period_cycles / (double)CLK_FREQUENCY);
        double duty = ((double)pulse_cycles/(double)period_cycles)*100.0;
        printk("Period Frequency: [%f] Hz\nPulse Cycles: [%f]\n", frequency, duty);
    }
}

static bool check_pwms_devices()
{

    if (device_is_ready(pwm.dev))
    {
        printk("Looks like he is ready!!\nDevice channel: {%d}\nDevice period: {%d}\n", pwm.channel, pwm.period);
    }
    else
    {
        printk("F is not ready yet\n");
        return false;
    }

    if (device_is_ready(cap.dev))
    {
        printk("Looks like out is ready!!\nDevice channel: {%d}\nDevice period: {%d}\n", cap.channel, cap.period);
    }
    else
    {
        printk("F out is not ready yet\n");
        return false;
    }

    return true;
}

static bool set_pwm_pulse_output_dft()
{
    int err;
    err = pwm_set(pwm.dev, pwm.channel, PWM_PERIOD_NSEC, PWM_PULSE_NSEC, PWM_POLARITY_NORMAL);
    if (err != 0)
    {
        printk("Error to set pwm out: Err{%d}\n", err);
        return false;
    }
    else
    {
        printk("PWM seted with a period of {%d} and pulse period of {%d}\n", PWM_PERIOD_NSEC, PWM_PULSE_NSEC);
        return true;
    }
}

static bool set_pwm_pulse_output_percent(uint8_t pulse_percent)
{
    int pulse_value;
    if(pulse_percent > 100){
        return false;
    }else{
        pulse_value = (pulse_percent*PWM_PERIOD_NSEC)/100;
    }
    int err;
    err = pwm_set(pwm.dev, pwm.channel, PWM_PERIOD_NSEC, pulse_value, PWM_POLARITY_NORMAL);
    if (err != 0)
    {
        printk("Error to set pwm out: Err{%d}\n", err);
        return false;
    }
    else
    {
        return true;
    }
}

int main(void)
{

    check_pwms_devices();
    //set_pwm_pulse_output_dft();

    pwm_configure_capture(cap.dev, cap.channel, (PWM_CAPTURE_TYPE_BOTH | PWM_CAPTURE_MODE_CONTINUOUS), capture_handler, NULL);
    pwm_enable_capture(cap.dev, cap.channel);
    uint8_t PULSES[21] = {1,5,10, 15, 20, 25, 30,35, 40, 45, 50, 55, 60, 65,70,75,80,85,90,95,100};

    while (1)
    {
        
        for (int i = 0; i < 21  ; i++)
        {
            //printk("\nDuty at [%d] percent\n",PULSES[i]);
            set_pwm_pulse_output_percent(PULSES[i]);
            k_msleep(500);
            set_pwm_pulse_output_percent(0);
            k_msleep(1000);
        }

        // k_sleep(K_SECONDS(1));

        // for (int i = 0; i < 11; i++)
        // {
        //     pwm_set_cycles(pwm.dev, pwm.channel, PERIODS[i], PERIODS[i]/2, PWM_POLARITY_NORMAL);
        //     k_msleep(300);
        // }
        k_sleep(K_SECONDS(1));
    }
    return 0;
}
