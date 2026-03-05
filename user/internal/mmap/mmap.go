package mmap

import (
	"fmt"
	"os"
	"syscall"
)

func Mmap() error {

	file, err := os.OpenFile("/dev/ws_epaper0", os.O_RDWR, 0)
	if err != nil {
		return fmt.Errorf("could not open /dev/wsepaper0 for RW: %v", err)
	}
	defer file.Close()

	buff, err := syscall.Mmap(int(file.Fd()), 0, 4096, syscall.PROT_READ|syscall.PROT_WRITE, syscall.MAP_SHARED)
	if err != nil {
		return fmt.Errorf("mmap failed: %v", err)
	}
	defer func() {
		err := syscall.Munmap(buff)
		if err != nil {
			panic(fmt.Sprintf("Could not unmap memory: %v", err))
		}
	}();

	buff[0] = 0xFF
	buff[1] = 0xFE
	buff[2] = 0xFD
	buff[3] = 0xFC

	buff[0xFF] = 0xAA

	buff[4080] = 0x55

	return nil
}
