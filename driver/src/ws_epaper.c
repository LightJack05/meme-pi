#include "linux/dev_printk.h"
#include "linux/mod_devicetable.h"
#include "linux/printk.h"
#include <linux/gpio/consumer.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spi/spi.h>

static struct spi_device *ws_spi_device;
static struct gpio_desc *data, *reset, *busy;

static void request_gpiod_pins(struct spi_device *spi){
    data = gpiod_get(&spi->dev, "dc", GPIOD_OUT_HIGH);
    reset = gpiod_get(&spi->dev, "rst", GPIOD_OUT_HIGH);
    busy = gpiod_get(&spi->dev, "bsy", GPIOD_IN);
    if (IS_ERR(data) || IS_ERR(reset) || IS_ERR(busy)) {
        panic("ws_epaper: Failed to request GPIO pins\n");
    }
}

static void initialize_gpio_pins(void){
    gpiod_set_value(reset, 0);
    gpiod_set_value(data, 1);
}

static int ws_epaper_probe(struct spi_device *spi) {
    dev_info(&spi->dev, "Probing device with CS: %u @ %u Hz",
             spi->chip_select[0], spi->max_speed_hz);
    dev_info(&spi->dev, "Device %s bound to driver.", spi->modalias);

    request_gpiod_pins(spi);

    spi->max_speed_hz = 1;
    int rc = spi_setup(spi);
    if (rc < 0) {
        dev_err(&spi->dev, "Failed to setup SPI device: %d\n", rc);
        return rc;
    }

    initialize_gpio_pins();

    ws_spi_device = spi;
    return 0;
}

static void ws_epaper_remove(struct spi_device *spi) {
    gpiod_put(data);
    gpiod_put(reset);
    gpiod_put(busy);
    ws_spi_device = NULL;
    pr_info("ws_epaper: Unbound from device\n");
}

static const struct of_device_id compatible_devices[] = {
    {.compatible = "waveshare,epaper-display"}, {}};

static const struct spi_device_id legacy_ids[] = {{"epaper-display", 0}, {}};

MODULE_DEVICE_TABLE(of, compatible_devices);
MODULE_DEVICE_TABLE(spi, legacy_ids);

static struct spi_driver spi_driver = {
    .driver =
        {
            .name = "waveshare-epaper-driver",
            .of_match_table = compatible_devices,
        },
    .id_table = legacy_ids,
    .probe = ws_epaper_probe,
    .remove = ws_epaper_remove,
};

module_spi_driver(spi_driver);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("LightJack05");
MODULE_DESCRIPTION("Hello, world module");
MODULE_VERSION("1.0");
