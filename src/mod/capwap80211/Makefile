include ../../Defs.mak


SOURCES=\
	mod_capwap80211.c \
	capwap80211_messages.c \


LIBDIR		:= ../../../lib
LIBARCHDIR	:= $(LIBDIR)/$(KERNEL)/$(ARCH)
OBJDIR		:= ../../../obj/mod_capwap80211/$(KERNEL)/$(ARCH)
SNAME		:= $(LIBARCHDIR)/libmod_capwap80211.a
DNAME		:= $(LIBARCHDIR)/mod_capwap80211.so
SLIBS		:= -lcw

CFLAGS+=-fPIC
CFLAGS+= -I ../../ -I../ -I../../include -I../../../include


all: $(SNAME) $(DNAME) 
static: $(SNAME)
dynamic: $(DNAME)

include ../../lib.mk


