.PHONY: all clean deploy

all:
	@echo "Compiling user-space program"
	mkdir -p ./bin/
	go build -v -o ./bin/meme-pi-user . 

clean:
	go clean .
	rm -rf ./bin/

deploy: all
	ssh meme-pi mkdir -p /meme-pi/bin
	scp ./bin/meme-pi-user meme-pi:/meme-pi/bin/meme-pi-user

