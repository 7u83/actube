include ../../Macros.mak
include ../../Config.mak

OBJS=\
	mod_capwap_ac.o \
	capwap_actions_ac.o \
	capwap_in_wtp_descriptor.o \
	capwap_in_wtp_board_data.o

OBJS:=$(patsubst %.o,$(ARCH)/%.o,$(OBJS))


CFLAGS = -Wall -g -O0 -D_REENTRANT -DWITH_IPV6 -DWITH_RMAC_SUPPORT -I ../../ -I../

NAME=libcapwap.a
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
