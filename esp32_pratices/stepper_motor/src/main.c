#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>

static const struct gpio_dt_spec dir = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(stepper), gpios, 0);
static const struct gpio_dt_spec steps = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(stepper), gpios, 1);

static int pos = 0;

static void move_to(int step)
{
    int error;

    if (step < pos)
    {
        gpio_pin_set_dt(&dir, 0);
        error = pos - step;
        for (int i = 0; i < error; i++)
        {
            gpio_pin_toggle_dt(&steps);
            pos--;
            k_usleep(500);
        }
    }
    else if (step > pos)
    {
        gpio_pin_set_dt(&dir, 1);
        error = step - pos;
        for (int i = 0; i < error; i++)
        {
            gpio_pin_toggle_dt(&steps);
            pos++;
            k_usleep(500);
        }
    }
    else
    {
        return;
    }
}

int main(void)
{
    int ret = 0;
    if (!device_is_ready(dir.port) || !device_is_ready(steps.port))
    {
        return 0;
    }
    else
    {
        ret = gpio_pin_configure_dt(&dir, GPIO_OUTPUT);
        if (ret < 0)
        {
            return 0;
        }
        ret = gpio_pin_configure_dt(&steps, GPIO_OUTPUT);
        if (ret < 0)
        {
            return 0;
        }
    }

    gpio_pin_set_dt(&dir, 1);

    while (1)
    {
        printk("Hello, Zephyr!\n");
        move_to(1600);
        k_msleep(100);
        move_to(800);
        k_msleep(100);
        move_to(100);
        k_msleep(100);
        move_to(0);
        break;
    }
    return 0;
}
