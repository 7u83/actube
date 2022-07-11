include ../Defs.mak

PROG=wtp
OBJDIR=./o
LIBDIR		:= ../../lib
LIBARCHDIR	:= $(LIBDIR)/$(KERNEL)/$(ARCH)

CFLAGS+=-I../ -DUSE_OPENSSL
LDFLAGS=-L$(LIBARCHDIR) 
LIBS+=-lcw -lnettle -lssl -ldl 

SOURCES=\
     	wtp_main.c\
	discovery.c\
	join.c\
	configure.c\
	run.c\
	changestate.c

include ../prog.mk
