.PHONY: clean deploy

all: wsepaper_main.c wsepaper_cleanup.c
	$(MAKE) -C ../kernel-headers.d/linux-6.12/ M=$(CURDIR) modules

debug: wsepaper_main.c wsepaper_cleanup.c
	$(MAKE) -C ../kernel-headers.d/linux-6.12/ M=$(CURDIR) modules KCFLAGS="-g"


clean:
	$(MAKE) -C ../kernel-headers.d/linux-6.12/ M=$(CURDIR) clean

deploy:
	ssh meme-pi mkdir -p /meme-pi/driver/
	bash -c 'ssh meme-pi "rmmod wsepaper || true" '
	ssh meme-pi rm -f /meme-pi/driver/wsepaper.ko
	scp ./wsepaper.ko meme-pi:/meme-pi/driver/
	ssh meme-pi insmod /meme-pi/driver/wsepaper.ko
