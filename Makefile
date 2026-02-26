.PHONY: all clean clean_module module deploy deploy_config deploy_driver deploy_overlay overlay reboot shell user deploy_user clean_user

all: overlay module user

clean: clean_module clean_user

clean_module:
	$(MAKE) -C ./driver/ clean

clean_user:
	$(MAKE) -C ./user/ clean

module:
	$(MAKE) -C ./driver/

user:
	$(MAKE) -C ./user/

deploy: deploy_overlay deploy_config deploy_driver deploy_user

deploy_user:
	$(MAKE) -C ./user/ deploy

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
