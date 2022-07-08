MF=stage.mk

PROGTARGET=$(OBJDIR)/$(PROG)


$(PROGTARGET): $(MF) $(SOURCES)
	@mkdir -p $(OBJDIR)
	@$(MAKE) -f $(MF)

$(MF): Makefile
	@rm -f $(MF)
	@echo "CFLAGS=$(CFLAGS)" >> $(MF)
	@echo "LDFLAGS=$(LDFLAGS)" >> $(MF)
	@echo -n "OBJS=" >> $(MF) 
	@for f in $(SOURCES) ; do \
		OF=$(OBJDIR)/`basename "$${f%.*}.o"` ; \
		echo -n " $${OF}" >> $(MF) ;\
	done
	@echo "" >> $(MF)
	@echo "$(PROGTARGET): \$$(OBJS)" >> $(MF) 
	@echo "	\$$(CC) -o $(PROGTARGET) \$$(LDFLAGS) \$$(OBJS) $(LIBS)" >> $(MF) 			
	@for f in $(SOURCES) ; do \
		OF=$(OBJDIR)/`basename "$${f%.*}.o"` ; \
		echo $${OF}: $$f  >> $(MF) ; \
		echo "	\$$(CC) -c \$$(CFLAGS) -o $${OF} $${f}" >> $(MF) ; \
	done


clean:
	rm -rf $(OBJDIR)
	rm -f $(PROGTARGET)
	rm -f $(MF)

install: $(PROGTARGET)
	mkdir -p $(INSTALL_BINDIR)
	install $(PROGTARGET) $(INSTALL_BINDIR)/$(PROG)

