#include "wsepaper_dev.h"
#include "linux/slab.h"
#include "wsepaper_data.h"
#include <linux/cdev.h>
#include <linux/kern_levels.h>
#include <linux/printk.h>

extern struct epaper_data *epaper_device;

// TODO: Add file operations
const struct file_operations epaper_fops = {.owner = THIS_MODULE};

static int register_device_file(void) {
    // This driver should only ever be able to have a single probe call. There
    // should be no possibility of multiple devices matching this drivers ID
    // table.
    int err = register_chrdev_region(MKDEV(0, 0), 0, "ws_epaper");
    if (err != 0) {
        printk(KERN_ERR "ws_epaper: Failed to allocate char device 0,0");
        return err;
    }
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

static int add_device_file(struct spi_device *spi) {
    printk(KERN_DEBUG "wsepaper: Adding device file at %p\n",
           epaper_device->cdev);
    int err = cdev_add(epaper_device->cdev, MKDEV(0, 0), 1);
    if (err != 0) {
        dev_err(&spi->dev, "Failed to add cdev to system: %d\n", err);
        return err;
    }
    return 0;
}

int setup_device_file(struct spi_device *spi) {
    printk(KERN_DEBUG "wsepaper: Initializing device at %p\n", epaper_device);
    int err = register_device_file();
    if (err != 0) {
        dev_err(&spi->dev, "Failed to register device file: %d\n", err);
        return err;
    }
    err = init_device_file(spi);
    if (err != 0) {
        dev_err(&spi->dev, "Failed to initialize device file: %d\n", err);
        return err;
    }
    err = add_device_file(spi);
    if (err != 0) {
        dev_err(&spi->dev, "Failed to add device file: %d\n", err);
        return err;
    }
    return 0;
}
