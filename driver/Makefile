all: kernel-headers.d

deploy:

kernel-headers.d: prepare_headers.sh
	bash prepare_headers.sh

headers_prepare: kernel-headers.d


headers-clean:
	rm -rf headers
