#include "wsepaper_dev.h"
#include "linux/fs.h"
#include "linux/slab.h"
#include "linux/spi/spi.h"
#include "linux/types.h"
#include "wsepaper_data.h"
#include "wsepaper_ioctl.h"
#include "wsepaper_mmap.h"
#include <linux/cdev.h>
#include <linux/kern_levels.h>
#include <linux/printk.h>

extern struct epaper_data *epaper_device;

// TODO: Add file operations
const struct file_operations epaper_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = ws_epaper_ioctl,
    .mmap = wsepaper_mmap,
};

static int register_device_file(dev_t *dev) {
    // This driver should only ever be able to have a single probe call. There
    // should be no possibility of multiple devices matching this drivers ID
    // table.
    int err = alloc_chrdev_region(dev, 0, 1, "ws_epaper");
    if (err != 0) {
        printk(KERN_ERR "wsepaper: Failed to allocate char device");
        return err;
    }
    printk(KERN_DEBUG "wsepaper: Allocated char device with major %x\n", *dev);
    return 0;
}

static int init_device_file(struct spi_device *spi) {
    epaper_device->cdev = kzalloc(sizeof(struct cdev), GFP_KERNEL);
    if (epaper_device->cdev == NULL) {
        dev_err(&spi->dev, "Failed to allocate memory for cdev structure\n");
        return -ENOMEM;
    }

    printk(KERN_DEBUG "wsepaper: Initializing device file at %p\n",
           epaper_device->cdev);
    cdev_init(epaper_device->cdev, &epaper_fops);
    if (epaper_device->cdev == NULL) {
        dev_err(&spi->dev, "Failed to initialize cdev structure\n");
        return -ENOMEM;
    }
    return 0;
}

static int add_device_file(struct spi_device *spi, dev_t dev) {
    printk(KERN_DEBUG "wsepaper: Adding device file at %p\n with dev %x\n",
           epaper_device->cdev, dev);
    int err = cdev_add(epaper_device->cdev, dev, 1);
    if (err != 0) {
        dev_err(&spi->dev, "Failed to add cdev to system: %d\n", err);
        return err;
    }
    return 0;
}

int setup_device_file(struct spi_device *spi) {
    printk(KERN_DEBUG "wsepaper: Initializing device at %p\n", epaper_device);
    dev_t dev = 0;
    int err = register_device_file(&dev);
    if (err != 0) {
        dev_err(&spi->dev, "Failed to register device file: %d\n", err);
        return err;
    }
    err = init_device_file(spi);
    if (err != 0) {
        dev_err(&spi->dev, "Failed to initialize device file: %d\n", err);
        return err;
    }
    err = add_device_file(spi, dev);
    if (err != 0) {
        dev_err(&spi->dev, "Failed to add device file: %d\n", err);
        return err;
    }
    epaper_device->dev = dev;
    return 0;
}
