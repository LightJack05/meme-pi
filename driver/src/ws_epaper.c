#include "linux/kern_levels.h"
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int __init ws_epaper_init(void) {
  printk(KERN_WARNING "module loaded");
  return 0;
}

static void __exit ws_epaper_exit(void) { printk(KERN_WARNING "module unloaded"); }

module_init(ws_epaper_init);
module_exit(ws_epaper_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LightJack05");
MODULE_DESCRIPTION("Hello, world module");
MODULE_VERSION("1.0");
