include ../Defs.mak

PROG=wtp
OBJDIR=./o
LIBDIR		:= ../../lib
LIBARCHDIR	:= $(LIBDIR)/$(KERNEL)/$(ARCH)

CFLAGS+=-I../ -DUSE_OPENSSL -I../../include -I/usr/include/libnl3
LDFLAGS+=-L$(LIBARCHDIR) -L/usr/local/lib
LIBS+=-lcw -lnettle -lssl -lcrypto -ldl -lpthread -lmavl -lwifi -lnl-3 -lnl-genl-3

SOURCES=\
     	wtp_main.c\
	discovery.c\
	join.c\
	configure.c\
	run.c\
	changestate.c \
	nlt.c

include ../prog.mk
