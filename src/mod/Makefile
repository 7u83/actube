all:
	$(MAKE) -C capwap
	$(MAKE) -C capwap80211
	$(MAKE) -C cisco
#	$(MAKE) -C fortinet
#	$(CC) -c modload_ac.c -I../
#	$(CC) -c modload_wtp.c -I../

clean:
	$(MAKE) -C capwap clean
	$(MAKE) -C capwap80211 clean
	$(MAKE) -C cisco clean
#	$(MAKE) -C fortinet clean
	rm -f *.o
