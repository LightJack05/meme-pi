#ifndef WSEPAPER_IOCTL_H
#define WSEPAPER_IOCTL_H

#include <linux/ioctl.h>
#include <linux/file.h>
#include "wsepaper_data.h"

extern struct epaper_data *epaper_device;

#define EP_TEST _IO(0x21, 1)

long ws_epaper_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

#endif // WSEPAPER_IOCTL_H
