include ../Defs.mak

PROG=wtp
OBJDIR=./o
LIBDIR		:= ../../lib
LIBARCHDIR	:= $(LIBDIR)/$(KERNEL)/$(ARCH)

CFLAGS+=-I../ -DUSE_OPENSSL -I../contrib/openssl-1.1.1f/include/ -I/home/tube/v/rsrch/libmavl
LDFLAGS+=-L$(LIBARCHDIR) 
LIBS+=-lcw -lnettle -lssl -lcrypto -ldl -lpthread -lmavl

SOURCES=\
     	wtp_main.c\
	discovery.c\
	join.c\
	configure.c\
	run.c\
	changestate.c

include ../prog.mk
