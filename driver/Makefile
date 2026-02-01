.PHONY: all clean distclean clean-src driver deploy headers_prepare clean-headers
all: kernel-headers.d driver

clean: clean-src 

distclean: clean-src clean-headers

clean-src:
	$(MAKE) -C ./src clean

driver:
	$(MAKE) -C ./src 

deploy:
	$(MAKE) -C ./src deploy

kernel-headers.d: prepare_headers.sh
	bash prepare_headers.sh

headers_prepare: kernel-headers.d

clean-headers:
	rm -rf kernel-headers.d/
