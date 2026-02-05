#include "wsepaper_class.h"
#include "linux/device.h"
#include "linux/device/class.h"
#include "linux/err.h"
#include "linux/spi/spi.h"
#include "wsepaper_data.h"

extern struct epaper_data *epaper_device;

static int register_epaper_class(void) {
    epaper_device->class = class_create("ws_epaper");
    if (IS_ERR(epaper_device->class)) {
        printk(KERN_ERR "wsepaper: Failed to create device class\n");
        return PTR_ERR(epaper_device->class);
    }
    printk(KERN_DEBUG "wsepaper: Created device class at %p\n",
           epaper_device->class);
    return 0;
}

static int register_epaper_device(struct spi_device *spi, dev_t dev) {
    struct device *epaper_class_device =
        device_create(epaper_device->class, &spi->dev, dev, NULL, "ws_epaper0");
    if (IS_ERR(epaper_class_device)) {
        dev_err(&spi->dev, "Failed to create device: %ld\n",
                PTR_ERR(epaper_class_device));
        return PTR_ERR(epaper_class_device);
    }
    epaper_device->device = epaper_class_device;
    printk(KERN_DEBUG "wsepaper: Created device at %p\n", epaper_class_device);
    return 0;
}

int setup_device(struct spi_device *spi, dev_t dev) {
    int err = register_epaper_class();
    if (err != 0) {
        dev_err(&spi->dev, "Failed to register device class: %d\n", err);
        return err;
    }
    err = register_epaper_device(spi, dev);
    if (err != 0) {
        dev_err(&spi->dev, "Failed to register device: %d\n", err);
        return err;
    }
    return 0;
}
