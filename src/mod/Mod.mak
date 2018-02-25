include ../../Macros.mak
include ../../Config.mak
include ../../Config.local.mak

OBJS:=$(patsubst %.o,$(OBJDIR)/%.o,$(OBJS))


CFLAGS = -fPIC -Wall -g -O0 -D_REENTRANT -DWITH_IPV6 $(COMPDEFS) -DWITH_RMAC_SUPPORT -I ../../ -I../ -I../../include


SRCS = $(OBJS:.o=.c) 

all: $(SNAME) $(DNAME)

$(OBJDIR)/%.o:%.c
	@mkdir -p $(OBJDIR)
	@echo "  $(CC) "$<
	@$(CC) -c $(CFLAGS) $< -o $@


$(SNAME) : $(OBJS) $(MODOBJS)
	@mkdir -p $(LIBDIR)
	@echo "  $(AR) $(SNAME)"
	@$(AR) rcs $(SNAME) $(OBJS) $(MODOBJS)

$(DNAME) : $(OBJS) $(MODOBJS)
	@mkdir -p $(LIBDIR)
	@echo "  $(CC) $(DNAME)"
	@$(CC) $(LDFLAGS) -shared -o $(DNAME) $(OBJS) $(MODOBJS) $(LIBS)

#	$(CC) -L$(LIBDIR) $(OBJS) $(MODOBJS) $(SLIBS) -v -shared -o ../../../lib/actube/capwap.so



clean: 
	rm -rf $(OBJDIR)
	rm -f $(SNAME)

