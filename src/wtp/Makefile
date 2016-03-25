include ../WtpConfig.mak
include ../Macros.mak

ifndef CC
	CC=gcc
endif


V=0

CFLAGS+=-DWITH_RMAC_SUPPORT 
CFLAGS+=-DWITH_IPV6
CFLAGS+=-DWITH_CW_LOG
CFLAGS+=-DWITH_CW_LOG_DEBUG
CFLAGS+=-DWITH_DTLS


ifndef ARCH
	ARCH = $(shell $(CC) -dumpmachine)
endif


CFLAGS += -O0 -Wall -Wno-unused-variable -Werror -g
#CFLAGS += -Os -Wall 
LDFLAGS += -L../contrib/jsmn -L../../src/lib/$(ARCH) 


CONFOBJS = wtp_conf.o


LIBS+=-lcapwap
LIBS+=-lcapwap80211
LIBS+=-lcisco
LIBS+=-lfortinet
LIBS+=-lcw
LIBS+=-lnl-3
LIBS+=-lnl-genl-3
LIBS+=-ljsmn



ifeq ($(CONF_LIBRARY),UCI)
CFLAGS+=-DWITH_UCI
LIBS+=-luci -lubox
CONFOBJS += conf_uci.o 
else
LIBS+=-lconfuse
CONFOBJS += wtp_conf_confuse.o
endif




ifeq ($(SSL_LIBRARY),GNUTLS)
CFLAGS+=-DWITH_GNUTLS
CFLAGS+=$(GNUTLS_CFLAGS)
LIBS+=$(GNUTLS_LIBS)
LDFLAGS+=$(GNUTLS_LDFLAGS)
endif

ifeq ($(SSL_LIBRARY),OPENSSL)
CFLAGS+=-DWITH_OPENSSL
CFLAGS+=$(OPESSL_CFLAGS)
LDFLAGS+=$(OPENSSL_LDFLAGS)
LIBS+=$(OPENSSL_LIBS)
endif




#HA_FILES += ../contrib/hostap/src/drivers/driver_nl80211.o

HA_INCS += -I$(INCLUDE_DIR)/libnl-tiny
HA_INCS += -I../contrib/hostap/src/utils
HA_INCS += -I../contrib/hostap/src

CFLAGS += $(HA_INCS)


#CFLAGS += -I../src
#CFLAGS += -I../src/utils
CFLAGS += -I../../src -I /usr/local/include -I ../contrib/jsmn

#CFLAGS += -I/usr/local/include




OBJS += wtp_main.o
OBJS += image_update.o
OBJS += cfg.o 


#OBJS += wtp_conf.o


OBJS += discovery.o
OBJS += wtp_interface.o
OBJS += wtpdrv.o
OBJS += dot11.o
OBJS += join.o
OBJS += run.o
OBJS += sulking.o
OBJS += configure.o
OBJS += changestate.o
OBJS += setup_conf.o
OBJS += $(CONFOBJS)


OBJS += nlt_attrnames.o
OBJS += nlt_cmdnames.o
OBJS += nlt.o

#OBJS += $(HA_FILES)

ALL=wtp

all: $(ALL)

Q=@
E=echo
ifeq ($(V), 1)
Q=
E=true
endif

%.o: %.c
	@$(E) "  CC " $<
	$(Q)$(CC) -c -o $@ $(CFLAGS) $<


wtp: $(OBJS)
	$(Q)$(CC) $(OBJS) ../mod/modload_wtp.o -o wtp $(LDFLAGS) $(LIBS)
	@$(E) "  LD " $@

clean:
#	$(MAKE) -C ../src clean
	rm -f *.o 
	rm -f *.d

-include $(OBJS:%.o=%.d)
