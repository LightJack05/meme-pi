#ifndef WSEPAPER_CLASS_H
#define WSEPAPER_CLASS_H

#include "linux/spi/spi.h"
#include "linux/types.h"

int setup_device(struct spi_device *spi, dev_t dev);

#endif // WSEPAPER_CLASS_H
