include ../Defs.mak

PROG=wtp
OBJDIR=./o
LIBDIR		:= ../../lib
LIBARCHDIR	:= $(LIBDIR)/$(KERNEL)/$(ARCH)

CFLAGS+=-I../ -DUSE_OPENSSL -I../../include
LDFLAGS+=-L$(LIBARCHDIR) -L/usr/local/lib
LIBS+=-lcw -lnettle -lssl -lcrypto -ldl -lpthread -lmavl

SOURCES=\
     	wtp_main.c\
	discovery.c\
	join.c\
	configure.c\
	run.c\
	changestate.c

include ../prog.mk
