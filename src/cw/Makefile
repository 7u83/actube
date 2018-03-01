include ../Macros.mak
include ../CWConfig.mak
-include ../Config.local.mak


LIBDIR		:= ../../lib
LIBARCHDIR	:= $(LIBDIR)/$(ARCH)
OBJDIR		:= ../../obj/cw/$(ARCH)

SNAME		:= $(LIBARCHDIR)/libcw.a
DNAME		:= $(LIBARCHDIR)/libcw.so

LDFLAGS+=-g -D_REENTRANT -L/usr/local/lib 

LIBS=

ifeq ($(WITH_OPENSSL),1)
CFLAGS+=$(OPENSSL_CFLAGS)
CFLAGS+=-DWITH_OPENSSL
DTLSOBJS += dtls_openssl.o \
	dtls_openssl_accept.o \
	dtls_openssl_connect.o \
	dtls_openssl_get_cipher.o \
	dtls_openssl_bio.o 
LIBS+=-lssl
endif

ifeq ($(WITH_GNUTLS),1)
CFLAGS+=$(GNUTLS_CFLAGS)
CFLAGS+=-DWITH_GNUTLS
DTLSOBJS+= dtls_gnutls.o \
	dtls_gnutls_accept.o \
	dtls_gnutls_connect.o \
	dtls_gnutls_bio.o \
	dtls_gnutls_get_cipher.o \
	dtls_gnutls_get_peers_cert.o
LIBS+=-lgnutls
endif


SRC=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRC))
#O:=$(OBJS);

OBJS:=$(patsubst %.o,$(OBJDIR)/%.o,$(OBJS))


all: $(SNAME) $(DNAME)


#CFLAGS = -D_XOPEN_SOURCE=500 -std=c90 -pedantic -Wall  -fPIC -g -O0 -D_REENTRANT -DWITH_IPV6 -DWITH_RMAC_SUPPORT -I /usr/local/include -I../
CFLAGS = -D_XOPEN_SOURCE=500 -D_BSD_SOURCE -Wall  -fPIC -g -O0 -D_REENTRANT -DWITH_IPV6 -DWITH_RMAC_SUPPORT -I /usr/local/include -I../

CFLAGS += $(GNUTLS_CFLAGS) \
	  -DWITH_CW_LOG \
	  -DWITH_CW_LOG_DEBUG \
	  -DWITH_DTLS \
	  $(XINCLUDE)\
	  -I $(OPENSSLINC)\
	-Werror 


$(OBJDIR)/%.o:%.c
	@mkdir -p $(OBJDIR)
	@echo "  $(CC) "$<
	@$(CC) -c $(CFLAGS) $< -o $@

$(SNAME) : $(OBJS) $(MODOBJS)
	@mkdir -p $(LIBARCHDIR)
	@echo "  AR $(SNAME)"
	@$(AR) rcs $(SNAME) $(OBJS) $(MODOBJS)

$(DNAME) : $(OBJS) $(MODOBJS)
	@mkdir -p $(LIBARCHDIR)
	@echo "  $(CC) $(DNAME)"
	@$(CC) $(LDFLAGS) -shared -o $(DNAME) $(OBJS) $(MODOBJS) $(LIBS)


SRCS = $(OBJS:.o=.c) 
DEPS := $(OBJS:.o=.d)


.PHONY: deps clean clean_libs libs



clean: 
	$(RM) $(OBJDIR)/*
	$(RM) $(DNAME)
	$(RM) $(SNAME)
	

clean_deps:
	$(DEPS) 
	
deps:  
	$(CC) -MM -E *.c $(CFLAGS) > .depend

-include .depend
