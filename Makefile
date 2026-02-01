.PHONY: all clean clean_module module deploy deploy_config deploy_driver deploy_overlay overlay reboot shell

all: overlay module

clean: clean_module

clean_module:
	$(MAKE) -C ./driver/ clean

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
