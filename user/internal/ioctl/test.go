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

	_, _, err = unix.Syscall(unix.SYS_IOCTL, devFile.Fd(), 0x2101, 0)

	devFile.Close()

	return nil
}
