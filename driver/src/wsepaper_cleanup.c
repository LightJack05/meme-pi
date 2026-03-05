#include "wsepaper_cleanup.h"
#include "linux/kern_levels.h"
#include "wsepaper_data.h"
#include "wsepaper_mmap.h"
#include <linux/cdev.h>
#include <linux/gpio/consumer.h>
#include <linux/printk.h>
#include <linux/spi/spi.h>
#include <linux/dma-mapping.h>

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
    unregister_device();
    unregister_class();
    delete_device_file();
    unregister_device_file();
    free_frame_buffer();
    free_epaper_device();
}

void free_epaper_device(void) {
    if (epaper_device == NULL) {
        return;
    }
    kfree(epaper_device);
    epaper_device = NULL;
}

void free_frame_buffer(void) {
    if (epaper_device->framebuffer == NULL) {
        return;
    }
    long buffer_size = (FRAMEBUFFER_NUM_PAGES) * (PAGE_SIZE);
    dma_free_coherent(&epaper_device->spi_dev->dev,buffer_size ,epaper_device->framebuffer, *epaper_device->dma_handle);
    epaper_device->framebuffer = NULL;
}

void unbind_device(void) {
    wsepaper_cleanup();
    pr_info("ws_epaper: Unbound from device\n");
}

void unregister_device(void) {
    if (epaper_device->device != NULL) {
        printk(KERN_DEBUG "Unregistering wsepaper device at %p\n",
               epaper_device);
        printk(KERN_DEBUG "Class: %p, Device: %p\n", epaper_device->class,
               epaper_device->device);
        printk(KERN_DEBUG "Unregistering device\n");
        printk(KERN_DEBUG "Device major: %d\n", MAJOR(epaper_device->dev));
        printk(KERN_DEBUG "Device minor: %d\n", MINOR(epaper_device->dev));
        device_destroy(epaper_device->class, epaper_device->dev);
        epaper_device->device = NULL;
    }
}

void unregister_class(void) {
    if (epaper_device->class != NULL) {
        class_destroy(epaper_device->class);
        epaper_device->class = NULL;
    }
}
