# meme-pi

> [!IMPORTANT]  
> This is very much an 'Under Construction' project that is meant to give me insight into how things work.

This is a toy project because I wanted to write a Linux driver for fun.

The plan:
- write a driver for a Waveshare 7.3" E-Ink display that runs as a module on my Raspberry Pi 3B+
- provide an ioctl and mmap based interface to userspace to allow it to control the display
- build a userspace application that is able to use the display to display memes, calendar, etc.

Current progress:
- ✅ Build toolchain to cross-compile the module on amd64 to aarch64 against an alpine Linux kernel
- ✅ Make the module loadable
- ✅ Make the module bind to the SPI device and make the probe function get called
- ✅ Bind the driver to the SPI device and set up the device for use
- ✅ Register a character device file
- ✅ Register a device class and device to the kernel and make it appear in /dev/
- 🔜 IN PROGRESS: Provide an IOCTL interface on the chrdev
- 🔜 Provide the mmap interface for mapping the kernel framebuffer to userspace
- 🔜 Send the buffer to the display so it appears on screen (+ reset/standby/etc. logic)
- 🔜 Add locking to ensure userspace does not send commands when the display is still busy (i.e. BSY is high)
- 🔜 Hook into the panic notifier chain so I don't brick the display if the module panics
