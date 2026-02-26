#include "wsepaper_ioctl.h"
#include "linux/kern_levels.h"
#include "linux/printk.h"
#include "linux/dev_printk.h"

extern struct epaper_data *epaper_device;

long ws_epaper_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
    case EP_REFRESH:
        dev_info(epaper_device->device, "Got request to refresh panel...");
        break;
    case EP_TEST:
        printk(KERN_INFO "ws_epaper: IOCTL received!\n");
        break;
    default:
        printk(KERN_ERR "ws_epaper: Unknown IOCTL command: 0x%x\n", cmd);
        return -EINVAL;
    }
    return 0;
}
