
all:
	$(MAKE) -C capwap
	$(MAKE) -C cipwap
	$(MAKE) -C cisco
	cc -c modload_ac.c -I../
	cc -c modload_wtp.c -I../

clean:
	$(MAKE) -C capwap clean
	$(MAKE) -C cipwap clean
	$(MAKE) -C cisco clean
	rm *.o
