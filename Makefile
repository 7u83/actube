.OBJDIR: ./

all:
	$(MAKE) -C src/cw -j16
	$(MAKE) -C src/mod -j16
	$(MAKE) -C src/ac -j16
	$(MAKE) -C src/wtp -j16
	

clean:
	$(MAKE) -C src/cw clean
	$(MAKE) -C src/mod clean
	$(MAKE) -C src/ac clean
	$(MAKE) -C src/wtp clean
	

