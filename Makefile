TARGETS=\
	src/contrib/jsmn \
	src/cw


#src/cw src/mod src/ac 




all:

#$(foreach i,$(TARGETS),$(call $(MAKE) -c, $i))

	$(MAKE) -C src/contrib/jsmn
	$(MAKE) -C src/cw
	$(MAKE) -C src/mod
	$(MAKE) -C src/ac
	

clean:

#$(foreach i,$(TARGETS),$(call $(MAKE) -c, $i))

	$(MAKE) -C src/contrib/jsmn clean
	$(MAKE) -C src/cw clean
	$(MAKE) -C src/mod clean
	$(MAKE) -C src/ac clean
	

