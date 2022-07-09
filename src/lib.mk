MF=Mf-$(KERNEL)-$(ARCH).mk

LIBTARGET=$(OBJDIR)/$(PROG)


$(SNAME): $(MF) $(SOURCES)
	@mkdir -p $(OBJDIR)
	@mkdir -p $(LIBARCHDIR)
	@$(MAKE) -f $(MF) $(SNAME)

$(DNAME): $(MF) $(SOURCES)
	@mkdir -p $(OBJDIR)
	@mkdir -p $(LIBARCHDIR)
	@$(MAKE) -f $(MF) $(DNAME)

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
	@echo "$(SNAME) : \$$(OBJS)" >> $(MF)
	@echo "	\$$(AR) rcs $(SNAME) \$$(OBJS)" >> $(MF)
	@echo "" >> $(MF)
	@echo "$(DNAME) : \$$(OBJS)" >> $(MF)
	@echo "	\$$(LD) \$$(LDFLAGS) -shared -o $(DNAME) \$$(OBJS) $(LIBS)" >> $(MF)
	@echo "" >> $(MF)
#	@echo "	\$$(CC) -o $(PROGTARGET) \$$(LDFLAGS) \$$(OBJS) $(LIBS)" >> $(MF) 			
	@for f in $(SOURCES) ; do \
		OF=$(OBJDIR)/`basename "$${f%.*}.o"` ; \
		echo $${OF}: $$f  >> $(MF) ; \
		echo "	\$$(CC) -c \$$(CFLAGS) -o $${OF} $${f}" >> $(MF) ; \
	done


clean:
	rm -rf $(OBJDIR)
	rm -f $(SNAME)
	rm -f $(DNAME)
	rm -f $(MF)

install: $(PROGTARGET)
	mkdir -p $(INSTALL_BINDIR)
	install $(PROGTARGET) $(INSTALL_BINDIR)/$(PROG)

