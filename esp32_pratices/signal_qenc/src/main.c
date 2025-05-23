#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/sensor.h>

static const struct pwm_dt_spec pwm = PWM_DT_SPEC_GET_BY_IDX(DT_NODELABEL(motor), 0);

static const struct pwm_dt_spec cap = PWM_DT_SPEC_GET_BY_IDX(DT_NODELABEL(motor), 1);

static const struct pwm_dt_spec cap_2 = PWM_DT_SPEC_GET_BY_IDX(DT_NODELABEL(motor), 2);

struct sensor_value val;
int rc;
const struct device *encoder = DEVICE_DT_GET(DT_ALIAS(qdec0));

#define PWM_PERIOD_NSEC 200000000

#define CLK_FREQUENCY 80000000

#define PULSE_COUTER_PER_REVOLUTION 11.0 // 11 pulse couter for 1 shaft revolution so 1/11 revolutions per pulse
#define GEAR_RATIO 4.4                   // Gear ratio of 4.4:1 so 4.4 revolutions for 1 shaft revolution

#define SHAFT_REVOLUTION_RATIO (PULSE_COUTER_PER_REVOLUTION * GEAR_RATIO)

#define PWM_POLARITY_INVERTED (1 << 0)
#define PWM_POLARITY_NORMAL (0 << 0)

#define FLAGS (PWM_CAPTURE_TYPE_PULSE | PWM_POLARITY_NORMAL)

static bool isForward;
static int prev_count = 0;
static int current_count = 0;

static int get_pulse_count()
{
    rc = sensor_sample_fetch_chan(encoder, SENSOR_CHAN_ROTATION);
    if (rc != 0)
    {
        printk("Failed to fetch sample (%d)\n", rc);
    }

    rc = sensor_channel_get(encoder, SENSOR_CHAN_ROTATION, &val);
    if (rc != 0)
    {
        printk("Failed to get data (%d)\n", rc);
    }

    return val.val1;
}

static void capture_handler_2(const struct device *dev, uint32_t channel, uint32_t period_cycles, uint32_t pulse_cycles, int status, void *user_data)
{
    if (status != 0)
    {
        printk("Error to capture - status: [%d]", status);
    }else{
        printk("Pulse from cap 1: %d\n",pulse_cycles);
    }
}

static void capture_handler(const struct device *dev, uint32_t channel, uint32_t period_cycles, uint32_t pulse_cycles, int status, void *user_data)
{
    if (status != 0)
    {
        printk("Error to capture - status: [%d]", status);
    }
    else
    {
        // printk("From device: %s\n", dev->name);
        // current_count = get_pulse_count();
        // if (current_count < prev_count)
        // {
        //     isForward = false;
        // }
        // else
        // {
        //     isForward = true;
        // }
        // prev_count = current_count;

        // double frequency = 1.0 / ((double)period_cycles / (double)CLK_FREQUENCY);

        double rpm = (frequency * 60.0) / (SHAFT_REVOLUTION_RATIO);

        // double duty = ((double)pulse_cycles / (double)period_cycles) * 100.0;
        printk("Pulse from cap 0: %d\n",pulse_cycles);

        // printk("%f,%f,%d,%.2f,%d\n", frequency, duty, current_count,rpm,isForward);
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
    if (device_is_ready(cap_2.dev))
    {
        printk("Looks like out is ready!!\nDevice channel: {%d}\nDevice period: {%d}\n", cap_2.channel, cap_2.period);
    }
    else
    {
        printk("F out is not ready yet\n");
        return false;
    }

    return true;
}

static bool set_pwm_pulse_output_percent(uint8_t pulse_percent)
{
    int pulse_value;
    if (pulse_percent > 100)
    {
        return false;
    }
    else
    {
        pulse_value = (pulse_percent * PWM_PERIOD_NSEC) / 100;
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
    pwm_configure_capture(cap.dev, cap.channel, (PWM_CAPTURE_TYPE_BOTH | PWM_CAPTURE_MODE_CONTINUOUS), capture_handler, NULL);
    pwm_configure_capture(cap_2.dev, cap_2.channel, (PWM_CAPTURE_TYPE_BOTH | PWM_CAPTURE_MODE_CONTINUOUS), capture_handler_2, NULL);
    pwm_enable_capture(cap.dev, cap.channel);
    pwm_enable_capture(cap_2.dev, cap_2.channel);

    if (!device_is_ready(encoder))
    {
        printk("Qdec device is not ready\n");
        return 0;
    }
    prev_count = get_pulse_count();
    set_pwm_pulse_output_percent(50);

    while (1)
    {

        k_sleep(K_SECONDS(1));
    }
    return 0;
}
