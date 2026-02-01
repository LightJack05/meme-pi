#ifndef WSEPAPER_DATA_H
#define WSEPAPER_DATA_H


struct epaper_data {
    struct cdev *cdev;
    struct spi_device *device;
    struct gpio_desc *data, *reset, *busy;
};

#endif // WSEPAPER_DATA_H
