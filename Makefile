default:
	echo "Not configured"

module:
	$(MAKE) -C ./driver/

deploy:
	$(MAKE) -C ./config/ deploy
