package main

import "github.com/LightJack05/meme-pi/user/internal/ioctl"

func main() {
	err := ioctl.Test()
	if err != nil {
		panic(err)
	}
}
