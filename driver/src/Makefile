.PHONY: clean deploy

all: ws_epaper.c
	$(MAKE) -C ../kernel-headers.d/linux-6.12/ M=$(CURDIR) modules

clean:
	$(MAKE) -C ../kernel-headers.d/linux-6.12/ M=$(CURDIR) clean

deploy:
	ssh meme-pi mkdir -p /meme-pi/driver/
	bash -c 'ssh meme-pi "rmmod ws_epaper || true" '
	ssh meme-pi rm -f /meme-pi/driver/ws_epaper.ko
	scp ./ws_epaper.ko meme-pi:/meme-pi/driver/
	ssh meme-pi insmod /meme-pi/driver/ws_epaper.ko
