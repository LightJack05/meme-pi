#include "wsepaper_ioctl.h"
#include "linux/kern_levels.h"
#include "linux/printk.h"

long ws_epaper_ioctl(struct file *file, unsigned int cmd,
          unsigned long arg) {
    switch (cmd) {
    case EP_TEST:
        printk(KERN_INFO "IOCTL received!");
        break;
    default:
        printk(KERN_ERR "Unknown IOCTL command: %u", cmd);
        return -EINVAL;
    }
    return 0;
}
