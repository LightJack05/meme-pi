all: overlay module

module:
	$(MAKE) -C ./driver/

deploy: deploy_overlay deploy_config deploy_driver

deploy_config:
	$(MAKE) -C ./config/ deploy

deploy_driver:
	$(MAKE) -C ./driver/ deploy

deploy_overlay:
	$(MAKE) -C ./overlay/ deploy

overlay:
	$(MAKE) -C ./overlay/ all

reboot:
	ssh meme-pi reboot

shell:
	sudo tio /dev/ttyUSB0
