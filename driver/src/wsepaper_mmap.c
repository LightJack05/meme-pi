#include "wsepaper_mmap.h"
#include "asm/memory.h"
#include "asm/page-def.h"
#include "asm/pgtable.h"
#include "linux/dev_printk.h"
#include "linux/mm.h"
#include "linux/spi/spi.h"
#include "wsepaper_data.h"
#include <asm-generic/errno-base.h>

extern struct epaper_data *epaper_device;

int wsepaper_mmap(struct file *file, struct vm_area_struct *vma) {
    unsigned long long physical_address;
    physical_address = virt_to_phys(epaper_device->framebuffer);

    vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

    long buffer_size = (FRAMEBUFFER_NUM_PAGES) * (PAGE_SIZE);

    long err =
        remap_pfn_range(vma, vma->vm_start, physical_address >> PAGE_SHIFT,
                        buffer_size, vma->vm_page_prot);
    if (err != 0) {
        dev_err(&epaper_device->spi_dev->dev, "Failed to mmap to user space.");
        return -EAGAIN;
    }

    return 0;
}
