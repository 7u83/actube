include ../Config.mak

#
# Define witch SSL library to use
# OPENSSL or GNUTLS
#
SSL_LIBRARY=OPENSSL

CFLAGS += -I/usr/include/libnl3
CFLAGS += -I../contrib/uci
LDFLAGS += -L../contrib/uci/build
LDFLAGS += -L../contrib/libubox/build



CONF_LIBRARY=UCI
USE_CONTRIB_UCI=0

ifeq ($(USE_CONTRIB_UCI),1)
CFLAGS+=-I../contrib/uci
endif


