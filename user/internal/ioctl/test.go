package ioctl

import (
	"fmt"
	"os"

	"golang.org/x/sys/unix"
)

func Test() error {
	devFile, err := os.OpenFile("/dev/ws_epaper0", os.O_RDWR, 0)
	if err != nil {
		return fmt.Errorf("Could not open /dev/ws_epaper0 for RW: %v", err)
	}

	_, _, errno := unix.Syscall(unix.SYS_IOCTL, devFile.Fd(), 0x2101, 0)
	if errno != 0 {
		return fmt.Errorf("ioctl failed: %v", errno.Error())
	}

	err = devFile.Close()
	if err != nil {
		panic(fmt.Sprintf("Could not close /dev/ws_epaper0: %v", err))
	}

	return nil
}
