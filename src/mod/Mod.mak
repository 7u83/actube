include ../../Macros.mak
include ../../Config.mak


OBJS:=$(patsubst %.o,$(ARCH)/%.o,$(OBJS))


CFLAGS = -Wall -g -O0 -D_REENTRANT -DWITH_IPV6 $(COMPDEFS) -DWITH_RMAC_SUPPORT -I ../../ -I../ -I../../include

LIBDIR=../../lib/$(ARCH)

SRCS = $(OBJS:.o=.c) 

$(ARCH)/%.o:%.c
	@mkdir -p $(ARCH)
	@echo "  CC "$<
	@$(CC) -c $(CFLAGS) $< -o $@


$(LIBDIR)/$(NAME) : $(OBJS) $(MODOBJS)
	@mkdir -p $(LIBDIR)
	@echo "  AR $(LIBDIR)/$(NAME)"
	@$(AR) rcs $(LIBDIR)/$(NAME) $(OBJS) $(MODOBJS)


all: $(LIBDIR)/$(NAME)

clean: 
	rm -rf $(ARCH)
	rm -f $(LIBDIR)/$(NAME)

