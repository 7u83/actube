include ../../../Macros.mak
include ../../../Config.mak

OBJS=\
	mod_cipwap_ac.o

OBJS:=$(patsubst %.o,$(ARCH)/%.o,$(OBJS))


CFLAGS = -Wall -g -O0 -D_REENTRANT -DWITH_IPV6 -DWITH_RMAC_SUPPORT -I ../../../


SRCS = $(OBJS:.o=.c) 

$(ARCH)/%.o:%.c
	@mkdir -p $(ARCH)
	@echo "  CC "$<
	@$(CC) -c $(CFLAGS) $< -o $@


all: $(OBJS)

clean: 
	rm -rf $(ARCH)
