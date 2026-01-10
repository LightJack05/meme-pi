all:

deploy: boot/cmdline.txt boot/config.txt boot/usercfg.txt
	scp -r boot/cmdline.txt meme-pi:/boot/cmdline.txt
	scp -r boot/config.txt meme-pi:/boot/config.txt
	scp -r boot/usercfg.txt meme-pi:/boot/usercfg.txt

