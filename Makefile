.OBJDIR: ./

all: ac wtp 
	

clean:
	$(MAKE) -C src/cw clean
	$(MAKE) -C src/mod clean
	$(MAKE) -C src/ac clean
	$(MAKE) -C src/wtp clean

cw:
	$(MAKE) -C src/cw 

ac: cw mod
	$(MAKE) -C src/ac 

wtp: cw mod
	$(MAKE) -C src/wtp 

mod: cw
	$(MAKE) -C src/mod 

