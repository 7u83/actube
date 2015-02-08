
#SSL settings
SSL_LIBRARY=OPENSSL
USE_CONTRIB_OPENSSL=1

# C compiler settings
CC=clang


CFLAGS += -I/usr/include/libnl3
CFLAGS += -I../contrib/uci
LDFLAGS += -L../contrib/uci/build
LDFLAGS += -L../contrib/libubox/build



CONF_LIBRARY=UCI
USE_CONTRIB_UCI=0


