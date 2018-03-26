include ../../Config.mak
-include ../../Config.local.mak
include ../../Macros.mak

OBJS:=$(patsubst %.o,$(OBJDIR)/%.o,$(OBJS))


#CFLAGS = -fPIC -Wall -g -O0 -D_REENTRANT -DWITH_IPV6 $(COMPDEFS) -DWITH_RMAC_SUPPORT 

CFLAGS+= -I ../../ -I../ -I../../include


SRCS = $(OBJS:.o=.c) 

all: $(SNAME) $(DNAME) $(MODNAME)

$(OBJDIR)/%.o:%.c
	@mkdir -p $(OBJDIR)
	@echo "  $(CC) "$<
	@$(CC) -c $(CFLAGS) $< -o $@


$(SNAME) : $(OBJS) $(MODOBJS)
	@mkdir -p $(LIBARCHDIR)
	@echo "  $(AR) $(SNAME)"
	@$(AR) rcs $(SNAME) $(OBJS) $(MODOBJS)

$(DNAME) : $(OBJS) $(MODOBJS)
	@mkdir -p $(LIBARCHDIR)
	@echo "  $(CC) $(DNAME)"
	@$(LD) -L$(LIBARCHDIR) $(LDFLAGS) -shared -o $(DNAME) $(OBJS) $(MODOBJS) $(SLIBS) $(LIBS)

$(MODNAME) : $(DNAME)
	cp $(DNAME) $(MODNAME)

#	$(CC) -L$(LIBARCHDIR) $(OBJS) $(MODOBJS) $(SLIBS) -v -shared -o ../../../lib/actube/capwap.so



clean: 
	rm -rf $(OBJDIR)
	rm -f $(SNAME)
	rm -f $(DNAME)
	rm -f $(MODNAME)

