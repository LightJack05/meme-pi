.PHONY: all clean deploy
all:
	dtc -@ -I dts -O dtb -o ws_epaper.dtbo ws_epaper.dts

clean:
	rm -f ws_epaper.dtbo

deploy:
	ssh meme-pi mkdir -p /meme-pi/overlay
	scp ./ws_epaper.dtbo meme-pi:/meme-pi/overlay/ws_epaper.dtbo
	ssh meme-pi cp /meme-pi/overlay/ws_epaper.dtbo /boot/overlays/
