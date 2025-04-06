#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>  
#include <zephyr/drivers/spi.h>

#define SPI_DEV DT_NODELABEL(spi_dev)

#define SPI_OPERATION (SPI_WORD_SET(8) | SPI_TRANSFER_MSB | SPI_OP_MODE_MASTER)

#define SPI_CS_DELAY 0

//static const struct spi_dt_spec spi = SPI_DT_SPEC_GET(SPI_DEV,SPI_OPERATION,SPI_CS_DELAY);


const struct spi_config spi_cfg = {
    .frequency = DT_PROP(DT_NODELABEL(spi1), clock_frequency),
    .operation = SPI_OP_MODE_MASTER | SPI_TRANSFER_MSB | SPI_WORD_SET(8),
    .cs = SPI_CS_CONTROL_INIT(DT_NODELABEL(spi1), 10),
};

const struct device *spi = DEVICE_DT_GET(SPI_DEV);



int main(void) {
    uint8_t cmd = 0x88;
    struct spi_buf tx_buffer = {.buf = &cmd, .len = 1};
    struct spi_buf_set buff_set = {.buffers = &tx_buffer, .count = 1};
    while (1) {
        spi_write (spi, &spi_cfg, &buff_set);
        k_sleep(K_MSEC(250));

    }
}
