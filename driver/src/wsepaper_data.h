#ifndef WSEPAPER_DATA_H
#define WSEPAPER_DATA_H

#include "linux/types.h"
#include <linux/dma-mapping.h>

struct epaper_data {
    struct cdev *cdev;
    struct spi_device *spi_dev;
    struct gpio_desc *data, *reset, *busy;
    struct class *class;
    struct device *device;
    unsigned char *framebuffer;
    dma_addr_t *dma_handle;
    dev_t dev;
};

#endif // WSEPAPER_DATA_H
