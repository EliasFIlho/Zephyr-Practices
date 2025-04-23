#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>

#define TIMER_PERIOD_AND_DURATION K_MSEC(20)
#define TIME_BASIS 0.02

static int16_t prev_pulse_count = 0;
static double velocity = 0.0;
const struct device *const dev = DEVICE_DT_GET(DT_ALIAS(qdec0));



struct k_timer calculate_velocity_tim;

static void tim_callback(struct k_timer *tim)
{
	struct sensor_value val;
	if (sensor_sample_fetch(dev) != 0)
	{
		printk("Failed to fetch sample\n");
	}
	else
	{
		sensor_channel_get(dev, SENSOR_CHAN_ROTATION, &val);
		velocity = (((double)(val.val1 - prev_pulse_count)) / (11.0*4.4))*(60.0/TIME_BASIS);
		prev_pulse_count = val.val1;
	}
}

int main(void)
{

	if (!device_is_ready(dev))
	{
		printk("Qdec device is not ready\n");
		return 0;
	}

	k_timer_init(&calculate_velocity_tim, tim_callback, NULL);
	k_timer_start(&calculate_velocity_tim,TIMER_PERIOD_AND_DURATION,TIMER_PERIOD_AND_DURATION);
	printk("Quadrature decoder sensor test\n");


	while (1)
	{
		printk("%d\n", prev_pulse_count);
	}
	return 0;
}
