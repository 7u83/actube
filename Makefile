.OBJDIR: ./

all:
	$(MAKE) -C src/cw
	$(MAKE) -C src/mod
	$(MAKE) -C src/ac
	$(MAKE) -C src/wtp
	

clean:
	$(MAKE) -C src/cw clean
	$(MAKE) -C src/mod clean
	$(MAKE) -C src/ac clean
	$(MAKE) -C src/wtp
	

