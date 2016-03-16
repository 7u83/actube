all:
	$(MAKE) -C src/contrib/jsmn
	$(MAKE) -C src/cw
	$(MAKE) -C src/mod
	$(MAKE) -C src/ac
	

