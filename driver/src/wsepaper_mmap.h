#ifndef WSEPAPER_MMAP_H
#define WSEPAPER_MMAP_H

#include <linux/fs.h>
#include <linux/mm.h>

#define FRAMEBUFFER_NUM_BYTES (800l * 480l) / 2l
#define FRAMEBUFFER_NUM_PAGES (FRAMEBUFFER_NUM_BYTES / PAGE_SIZE) + 1

int wsepaper_mmap(struct file *file, struct vm_area_struct *vma);

#endif // WSEPAPER_MMAP_H
