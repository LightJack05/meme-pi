#include "asm/page-def.h"
#include "linux/dev_printk.h"
#include "linux/err.h"
#include "linux/gfp_types.h"
#include "linux/mod_devicetable.h"
#include "linux/slab.h"
#include "linux/stddef.h"
#include "linux/types.h"
#include "wsepaper_class.h"
#include "wsepaper_cleanup.h"
#include "wsepaper_data.h"
#include "wsepaper_dev.h"
#include "wsepaper_mmap.h"
#include <asm-generic/errno-base.h>
#include <linux/dma-mapping.h>
#include <linux/fs.h>
#include <linux/gpio/consumer.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spi/spi.h>

struct epaper_data *epaper_device = NULL;

static unsigned long request_gpiod_pins(struct spi_device *spi) {
    epaper_device->data = gpiod_get(&spi->dev, "dc", GPIOD_OUT_HIGH);
    epaper_device->reset = gpiod_get(&spi->dev, "rst", GPIOD_OUT_HIGH);
    epaper_device->busy = gpiod_get(&spi->dev, "bsy", GPIOD_IN);

    if (IS_ERR(epaper_device->data)) {
        dev_err(&spi->dev, "GPIO Pin request failed for pin: dc");
        return -EBUSY;
    }
    if (IS_ERR(epaper_device->reset)) {
        dev_err(&spi->dev, "GPIO Pin request failed for pin: rst");
        return -EBUSY;
    }
    if (IS_ERR(epaper_device->busy)) {
        dev_err(&spi->dev, "GPIO Pin request failed for pin: bsy");
        return -EBUSY;
    }

    return 0;
}

static void initialize_gpio_pins(void) {
    gpiod_set_value(epaper_device->reset, 0);
    gpiod_set_value(epaper_device->data, 1);
}

static unsigned long allocate_framebuffer(void) {
    dma_addr_t *dma_handle = NULL;
    if (epaper_device->framebuffer != NULL) {
        dev_err(&epaper_device->spi_dev->dev,
                "Expected framebuffer to be NULL, got %p\n",
                epaper_device->framebuffer);
        return -EBUSY;
    }

    dev_info(&epaper_device->spi_dev->dev,
             "Current page size: %ld\nFRAMEBUFFER_NUM_PAGES: "
             "%ld\nFRAMEBUFFER_NUM_BYTES: %ld",
             PAGE_SIZE, FRAMEBUFFER_NUM_PAGES, FRAMEBUFFER_NUM_BYTES);

    long buffer_size = (FRAMEBUFFER_NUM_PAGES) * (PAGE_SIZE);
    dev_info(&epaper_device->spi_dev->dev,
             "Allocating new framebuffer with size %ld\n", buffer_size);

    unsigned char *new_buffer = dma_alloc_coherent(
        &epaper_device->spi_dev->dev, buffer_size, dma_handle, GFP_KERNEL);
    if (new_buffer == NULL) {
        dev_err(&epaper_device->spi_dev->dev,
                "Failed to allocate buffer, pointer: %p\n", new_buffer);
        return -ENOMEM;
    }

    epaper_device->framebuffer = new_buffer;
    epaper_device->dma_handle = dma_handle;

    return 0;
}

static int ws_epaper_probe(struct spi_device *spi) {
    // Print some debug info
    dev_info(&spi->dev, "Probing device with CS: %u @ %u Hz",
             spi->chip_select[0], spi->max_speed_hz);
    dev_info(&spi->dev, "Device %s bound to driver.", spi->modalias);

    // Check if the device has already been probed
    // This shouldn't happen, but who knows.
    if (epaper_device != NULL) {
        dev_crit(&spi->dev,
                 "Already probed a device, there should not be a second one?!");
        return -EBUSY;
    }
    // Allocate the device struct
    struct epaper_data *epaper_data_ptr =
        kzalloc(sizeof(struct epaper_data), GFP_KERNEL);
    if (epaper_data_ptr == NULL) {
        dev_err(&spi->dev, "Failed to allocate memory for epaper_data");
        return -ENOMEM;
    }
    epaper_device = epaper_data_ptr;

    // Request GPIO pins for the device
    unsigned long err = request_gpiod_pins(spi);
    if (err != 0) {
        dev_err(&spi->dev, "Failed to request GPIO pins: %ld\n", err);
        wsepaper_cleanup();
        return err;
    }

    // Set up the SPI device
    spi->max_speed_hz = 1;
    err = spi_setup(spi);
    if (err < 0) {
        dev_err(&spi->dev, "Failed to setup SPI device: %ld\n", err);
        wsepaper_cleanup();
        return err;
    }

    // Register the device in the module
    epaper_device->spi_dev = spi;

    // Initialize the GPIO pins
    initialize_gpio_pins();

    // Allocate the framebuffer for the display
    err = allocate_framebuffer();
    if (err != 0) {
        dev_err(&spi->dev, "Framebuffer allocation failed: %ld\n", err);
        wsepaper_cleanup();
        return err;
    }

    printk(KERN_DEBUG "ws_epaper: epaper_device is at %p\n", epaper_device);
    // Set up the device file
    err = setup_device_file(spi);
    if (err != 0) {
        dev_err(&spi->dev, "Failed to setup device file: %ld\n", err);
        wsepaper_cleanup();
        return err;
    }

    err = setup_device(spi, epaper_device->dev);
    if (err != 0) {
        dev_err(&spi->dev, "Failed to setup device: %ld\n", err);
        wsepaper_cleanup();
        return err;
    }

    return 0;
}

static void ws_epaper_remove(struct spi_device *spi) { unbind_device(); }

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
MODULE_DESCRIPTION("WaveShare Epaper Driver");
MODULE_VERSION("1.0");
