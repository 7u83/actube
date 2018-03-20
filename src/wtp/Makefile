include ../Defs.mak
include ../Config.mak
include ../WtpConfig.mak
-include ../Config.local.mak
-include ../WtpConfig.local.mak
include ../Macros.mak



LIBDIR		:= ../../lib
LIBARCHDIR	:= $(LIBDIR)/$(ARCH)
OBJDIR		:= ../../obj/wtp/$(ARCH)
EXENAME		:= ./wtp

LIBS		:= -lcw
#STATICLIBS	:= $(LIBDIR)/$(ARCH)/libcw.a
LDFLAGS		+= -L$(LIBARCHDIR)

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


#SRC=$(wildcard *.c)
SRC=wtp_main.c discovery.c

OBJS=$(patsubst %.c,%.o,$(SRC))
OBJS:=$(patsubst %.o,$(OBJDIR)/%.o,$(OBJS))

ALL=$(EXENAME)

all: $(ALL)


$(OBJDIR)/%.o:%.c
	@mkdir -p $(OBJDIR)
	@echo "  $(CC) "$<
	@$(CC) -c $(CFLAGS) $< -o $@

$(EXENAME): $(OBJS)
	@echo "  $(LD) $(EXENAME)"
	@$(LD) $(LDFLAGS) -o $(EXENAME) $(OBJS) $(LIBS) $(STATICLIBS)


clean: 
	$(RM) $(OBJDIR)/*
	$(RM) $(EXENAME)

deps:  
	echo "# DO NOT DELETE THIS LINE -- make depend depends on it." > .depend
	makedepend $(SRC) -f.depend -p$(OBJDIR)/


-include .depend
