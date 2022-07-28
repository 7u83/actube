include ../../Defs.mak


SOURCES=\
	mod_capwap_ac.c \
	capwap_actions_ac.c \
	capwap_in_wtp_board_data.c \
	capwap_in_ac_descriptor.c \
	capwap_out_wtp_board_data.c \
	capwap_out_ac_descriptor.c \
	capwap_out_get_idle_timeout.c \
	capwap_out_wtp_descriptor.c \
	capwap_in_wtp_descriptor.c \
	capwap_out_ac_ip_list.c \
	capwap_in_vendor_specific_payload.c \
	capwap_in_mtu_discovery_padding.c \
	capwap_out_capwap_control_ip_address.c \
	capwap_in_capwap_control_ip_address.c \
	capwap_out_capwap_local_ip_address.c\


LIBDIR		:= ../../../lib
LIBARCHDIR      := $(LIBDIR)/$(KERNEL)/$(ARCH)
OBJDIR          := ../../../obj/mod_capwap/$(KERNEL)/$(ARCH)

SNAME		:= $(LIBARCHDIR)/libmod_capwap.a
DNAME		:= $(LIBARCHDIR)/mod_capwap.so
#SLIBS		:= -lcw


CFLAGS+=-fPIC
CFLAGS+= -I ../../ -I../ -I../../include -I../../../include
CFLAGS+=$(INC2)

all: $(SNAME) $(DNAME) 
static: $(SNAME)
dynamic: $(DNAME)
	

include ../../lib.mk



