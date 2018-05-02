include ../Defs.mak

include ../Config.mak
include ../ACConfig.mak
-include ../Config.local.mak
-include ../ACConfig.local.mak
include ../Macros.mak


LIBARCHDIR=../../lib/$(ARCH)
INCL= -I/usr/local/include -I../
LDFLAGS+=-L$(LIBARCHDIR) 
#CFLAGS += -W -g -O0 -D_REENTRANT -DIPV6 $(INCL) -I../ -DSYS_ARCH=\"${ARCH}\"

CFLAGS+=-DSYS_ARCH=\"$(ARCH)\"

LIBS+=-lcw
LIBS+=-lrt
LIBS+=-lpthread
LIBS+=-lconfuse
LIBS+=-lsqlite3
#LIBS+=-ldl


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

#AC_MODULES = mod_cipwap.o


# list of generated object files for AC. 
AC_OBJS = \
	wtplist.o wtpman.o conf.o ac_main.o \
	dataman.o \
	ac_interface.o \
	socklist.o \
	db.o \
	ac_global.o \
	discovery_cache.o\
	shell.o\
	${AC_MODULES}



AC_SRCS = $(AC_OBJS:.o=.c) 
AC_DEPS := $(AC_OBJS:.o=.d)

AC_NAME = actube

.PHONY: deps clean clean_libs libs

# top-level rule, to compile everything. 
all: $(AC_NAME) 

%.o:%.c
	@echo "  $(CC) "$<
	@$(CC) -c $(CFLAGS) $< -o $@


$(AC_NAME): $(AC_OBJS) 
#	$(CC) $(AC_OBJS) ../mod/modload_ac.o -o $(AC_NAME) $(LDFLAGS) $(LIBS)
	$(LD) $(AC_OBJS) -o $(AC_NAME) $(LDFLAGS) $(LIBS) 


clean: 
	$(RM) $(AC_NAME) $(AC_OBJS) $(AC_DEPS)

clean_deps:
	$(AC_DEPS) 
	
deps:  
	echo "# DO NOT DELETE THIS LINE -- make depend depends on it." > .depend
#	makedepend $(AC_SRC) -f.depend -p$(OBJDIR)/
	makedepend *.c -f.depend


-include .depend

