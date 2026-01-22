#include "linux/mod_devicetable.h"
#include "linux/printk.h"
#include <linux/gpio/consumer.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spi/spi.h>

static struct spi_device *ws_spi_device;

static int ws_epaper_probe(struct spi_device *spi) {
    pr_info("ws_epaper: Bound to device\n");
    return 0;
}

static void ws_epaper_remove(struct spi_device *spi) {
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
