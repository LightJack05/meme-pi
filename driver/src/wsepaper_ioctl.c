#include "wsepaper_ioctl.h"
#include "asm/page-def.h"
#include "linux/dev_printk.h"
#include "linux/kern_levels.h"
#include "linux/printk.h"
#include "linux/spi/spi.h"

extern struct epaper_data *epaper_device;

static void ws_epaper_dump_framebuffer_head(void) {
    unsigned long long *buff_start = (unsigned long long*)epaper_device->framebuffer;

    for (unsigned long long *buff_ptr = buff_start; buff_ptr < buff_start + (PAGE_SIZE) * 2; buff_ptr += 1 * 8) {
        dev_info(&epaper_device->spi_dev->dev, "0x %llx %llx %llx %llx %llx %llx %llx %llx", buff_ptr[0], buff_ptr[1], buff_ptr[2], buff_ptr[3], buff_ptr[4], buff_ptr[5], buff_ptr[6], buff_ptr[7]);
    }
}

long ws_epaper_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
    case EP_REFRESH:
        dev_info(epaper_device->device, "Got request to refresh panel...");
        break;
    case EP_TEST:
        printk(KERN_INFO "ws_epaper: IOCTL received!\n");
        ws_epaper_dump_framebuffer_head();
        break;
    default:
        printk(KERN_ERR "ws_epaper: Unknown IOCTL command: 0x%x\n", cmd);
        return -EINVAL;
    }
    return 0;
}

