#include "wsepaper_cleanup.h"
#include "wsepaper_data.h"
#include <linux/cdev.h>
#include <linux/gpio/consumer.h>
#include <linux/printk.h>
#include <linux/spi/spi.h>

extern struct epaper_data *epaper_device;

void free_gpiod_pins(void) {
    gpiod_put(epaper_device->data);
    gpiod_put(epaper_device->reset);
    gpiod_put(epaper_device->busy);
}

void unregister_device_file(void) { unregister_chrdev_region(MKDEV(0, 0), 0); }
void delete_device_file(void) {
    if (epaper_device->cdev != NULL) {
        cdev_del(epaper_device->cdev);
        epaper_device->cdev = NULL;
    }
    kfree(epaper_device->cdev);
}

void wsepaper_cleanup(void) {
    free_gpiod_pins();
    delete_device_file();
    unregister_device_file();
    free_epaper_device();
}

void free_epaper_device(void) {
    if (epaper_device == NULL) {
        return;
    }
    kfree(epaper_device);
    epaper_device = NULL;
}

void unbind_device(void) {
    wsepaper_cleanup();
    pr_info("ws_epaper: Unbound from device\n");
}
