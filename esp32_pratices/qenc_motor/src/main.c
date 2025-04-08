#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>  


int main(void) {

    struct sensor_value val;
	int rc;
	const struct device *const dev = DEVICE_DT_GET(DT_ALIAS(qdec0));

    if (!device_is_ready(dev)) {
		printk("Qdec device is not ready\n");
		return 0;
	}

    printk("Quadrature decoder sensor test\n");
    int test = 140;
    while (1) {
        rc = sensor_sample_fetch(dev);
		if (rc != 0) {
			printk("Failed to fetch sample (%d)\n", rc);
			return 0;
		}

		rc = sensor_channel_get(dev, SENSOR_CHAN_ROTATION, &val);
		if (rc != 0) {
			printk("Failed to get data (%d)\n", rc);
			return 0;
		}

		printk("%d,%d\n", val.val1,test);
    }
    return 0;
}
