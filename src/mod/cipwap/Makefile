include ../../Macros.mak
include ../../Config.mak
include ../../Config.local.mak

OBJS=\
	mod_cipwap_ac.o \
	cipwap_actions_ac.o \
	cipwap_actions_wtp.o \
	cipwap_items.o \

NAME		:= cipwap

LIBDIR		:= ../../../lib
LIBARCHDIR	:= $(LIBDIR)/$(ARCH)
OBJDIR		:= ../../../obj/mod_$(NAME)/$(ARCH)
SNAME		:= $(LIBARCHDIR)/libmod_$(NAME).a
DNAME		:= $(LIBARCHDIR)/mod_$(NAME).so
MODNAME		:= $(LIBDIR)/mod_$(NAME).so
SLIBS		:= -lcw 


include ../Mod.mak

