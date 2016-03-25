
include ../ACConfig.mak
include ../Macros.mak

ifndef CC
        CC=gcc
endif
#SYSARCH := $(shell uname -m)
ifndef ARCH
	ARCH = $(shell $(CC) -dumpmachine)
endif

LDFLAGS+=-g -D_REENTRANT -L/usr/local/lib -L../lib/$(ARCH) 
CFLAGS += -Werror -g -O0 -D_REENTRANT -DIPV6 -I/usr/local/include -I../ -DSYS_ARCH=\"${ARCH}\"


LIBS+=-lcapwap
LIBS+=-lcisco
LIBS+=-lfortinet
LIBS+=-lcipwap
LIBS+=-lcapwap80211
LIBS+=-lcw
LIBS+=-lrt
LIBS+=-lpthread
LIBS+=-lconfuse
LIBS+=-lsqlite3


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

CFLAGS += -DWITH_CW_LOG
CFLAGS += -DWITH_CW_LOG_DEBUG

CFLAGS += -DWITH_RMAC_SUPPORT
CFLAGS += -DWITH_DTLS
CFLAGS += -DWITH_IPV6
#CFLAGS += -DWITH_LWAPP

RM = /bin/rm -f 

AC_MODULES = mod_cipwap.o


# list of generated object files for AC. 
AC_OBJS = \
	wtplist.o wtpman.o conf.o ac_main.o \
	ac_interface.o \
	socklist.o \
	db.o \
	ac_global.o \
	${AC_MODULES}



AC_SRCS = $(AC_OBJS:.o=.c) 
AC_DEPS := $(AC_OBJS:.o=.d)

AC_NAME = actube

.PHONY: deps clean clean_libs libs

# top-level rule, to compile everything. 
all: $(AC_NAME) 

%.o:%.c
	@echo "  CC "$<
	@$(CC) -c $(CFLAGS) $< -o $@


$(AC_NAME): $(AC_OBJS) 
	$(CC) $(AC_OBJS) ../mod/modload_ac.o -o $(AC_NAME) $(LDFLAGS) $(LIBS)


clean: 
	$(RM) $(AC_NAME) $(AC_OBJS) $(AC_DEPS)

clean_deps:
	$(AC_DEPS) 
	
deps: $(AC_SRC) 
	$(CC) -MD -E $(AC_SRCS) $(CFLAGS) >/dev/null

-include $(AC_DEPS)
