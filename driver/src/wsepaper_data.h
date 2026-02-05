#ifndef WSEPAPER_DATA_H
#define WSEPAPER_DATA_H

#include "linux/types.h"
struct epaper_data {
    struct cdev *cdev;
    struct spi_device *spi_dev;
    struct gpio_desc *data, *reset, *busy;
    struct class *class;
    struct device *device;
    dev_t dev;
};

#endif // WSEPAPER_DATA_H
