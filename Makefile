all: module

module:
	$(MAKE) -C ./driver/

deploy: deploy_config deploy_driver

deploy_config:
	$(MAKE) -C ./config/ deploy

deploy_driver:
	$(MAKE) -C ./driver/ deploy

reboot:
	ssh meme-pi reboot

shell:
	sudo tio /dev/ttyUSB0
