include ../Defs.mak

PROG=wtp
OBJDIR=./o
LIBDIR		:= ../../lib
LIBARCHDIR	:= $(LIBDIR)/$(ARCH)

CFLAGS+=-I../ -DWITH_GNUTLS
LDFLAGS=-L$(LIBARCHDIR) 
LIBS=-lcw -lnettle -lgnutls -ldl

SOURCES=\
     	wtp_main.c\
	discovery.c\
	join.c\
	configure.c\
	run.c\
	changestate.c



include ../prog.mk
