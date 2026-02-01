#ifndef WSEPAPER_DEV_H
#define WSEPAPER_DEV_H

#include <linux/spi/spi.h>

int setup_device_file(struct spi_device *spi);

#endif // WSEPAPER_DEV_H
