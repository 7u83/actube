ifndef CC
CC=gcc
endif

CFLAGS+=-DWITH_RMAC_SUPPORT 
CFLAGS+=-DWITH_IPV6
CFLAGS+=-DWITH_CW_LOG
CFLAGS+=-DWITH_CW_LOG_DEBUG
CFLAGS+=-DWITH_DTLS


SYSARCH := $(shell uname -m)
ifndef ARCH
        ARCH=$(SYSARCH)
endif

X=$(shell echo $(ARCH))

#ifndef CFLAGS
CFLAGS += -O2 -Wall -g
#endif

LDFLAGS += -L../../src/capwap/$(ARCH)
LDFLAGS += -L/usr/local/lib
#LDFLAGS += -lpthread
#LDFLAGS += -lrt

LIBS+=-lcapwap
#LIBS+=-liw
LIBS+=-lssl
LIBS+=-lcrypto
LIBS+=-lpthread
LIBS+=-lrt
LIBS+=-lconfuse


CFLAGS += -I../src
CFLAGS += -I../src/utils
CFLAGS += -I../../src
CFLAGS += -I/usr/local/include

OBJS += wtp_main.o
OBJS += wtp_conf.o
OBJS += discovery.o
OBJS += wtp_interface.o
OBJS += wtpdrv.o
OBJS += join.o
OBJS += run.o
OBJS += sulking.o

ALL=wtp

all: $(ALL)

Q=@
E=echo
ifeq ($(V), 1)
Q=
E=true
endif

%.o: %.c
	$(Q)$(CC) -c -o $@ $(CFLAGS) $<
	@$(E) "  CC " $<


wtp: $(BCHECK) $(OBJS)
	$(Q)$(CC) $(LDFLAGS) -o wtp $(OBJS) $(LIBS)
	
	@$(E) "  LD " $@

clean:
#	$(MAKE) -C ../src clean
	rm -f *.o 
	rm -f *.d

-include $(OBJS:%.o=%.d)
