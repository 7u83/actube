include ../WtpConfig.mak
include ../Macros.mak

ifndef CC
	CC=gcc
endif

ifdef INCLUDE_DIR
	XINCLUDE=-I $(INCLUDE_DIR)
endif

ifdef LIB_DIR
	XLIB=-L $(LIB_DIR)
endif 
include ../Config.mak


ifeq ($(USE_CONTRIB_UCI),1)
CFLAGS+=-I../contrib/uci
endif


WITH_UCI=1 

CFLAGS+=-DWITH_RMAC_SUPPORT 
CFLAGS+=-DWITH_IPV6
CFLAGS+=-DWITH_CW_LOG
CFLAGS+=-DWITH_CW_LOG_DEBUG
CFLAGS+=-DWITH_DTLS
CFLAGS+=-DWITH_UCI
CFLAGS+=$(XINCLUDE)


ifndef ARCH
	ARCH = $(shell $(CC) -dumpmachine)
endif


#ifndef CFLAGS
CFLAGS += -O2 -Wall -g
#endif

LDFLAGS += -L../../src/capwap/$(ARCH)
#LDFLAGS += -L/usr/local/lib
LDFLAGS += -luci
#LDFLAGS += -L../capwap
LDFLAGS += $(XLIB)

#LDFLAGS += -lpthread
#LDFLAGS += -lrt

LIBS+=-lcapwap
#LIBS+=-liw
LIBS+=-lnl-3
LIBS+=-lnl-genl-3
LIBS+=$(OPENSSLLIB)
#LIBS+=-lcrypto
#LIBS+=-lrt
ifdef WITH_UCI
LIBS+=-luci -lubox
else
LIBS+=-lconfuse
endif

#HA_FILES += ../contrib/hostap/src/drivers/driver_nl80211.o

HA_INCS += -I$(INCLUDE_DIR)/libnl-tiny
HA_INCS += -I../contrib/hostap/src/utils
HA_INCS += -I../contrib/hostap/src

CFLAGS += $(HA_INCS)


#CFLAGS += -I../src
#CFLAGS += -I../src/utils
CFLAGS += -I../../src
CFLAGS += -I$(OPENSSLINC)

#CFLAGS += -I/usr/local/include



CONFOBJS = wtp_conf.o

ifdef WITH_UCI
	CONFOBJS += conf_uci.o 
else
	CONFOBJS += wtp_conf_confuse.o
endif 

OBJS += wtp_main.o

#OBJS += wtp_conf.o


OBJS += discovery.o
OBJS += wtp_interface.o
OBJS += wtpdrv.o
OBJS += dot11.o
OBJS += join.o
OBJS += run.o
OBJS += sulking.o
OBJS += configure.o
OBJS += $(CONFOBJS)


OBJS += nlt_attrnames.o
OBJS += nlt_cmdnames.o
OBJS += nlt.o

#OBJS += $(HA_FILES)

ALL=wtp

all: $(ALL)

Q=
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
