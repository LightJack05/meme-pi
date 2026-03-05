package main

import (
	"github.com/LightJack05/meme-pi/user/internal/ioctl"
	"github.com/LightJack05/meme-pi/user/internal/mmap"
)

func main() {
	err := mmap.Mmap()
	if err != nil {
		panic(err)
	}
	err = ioctl.Test()
	if err != nil {
		panic(err)
	}
}
