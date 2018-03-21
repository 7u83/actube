include ../../Defs.mak


OBJS=\
	mod_capwap_ac.o \
	capwap_actions_ac.o \
	capwap_in_wtp_board_data.o \
	capwap_out_wtp_board_data.o \
	capwap_out_ac_descriptor.o \
	capwap_out_get_session_id.o \
	capwap_out_get_idle_timeout.o \
	capwap_out_wtp_descriptor.o \
	capwap_in_wtp_descriptor.o \
	capwap_out_ac_ip_list.o \
	capwap_in_session_id.o \
	capwap_in_vendor_specific_payload.o \
	capwap_in_mtu_discovery_padding.o \
	capwap_out_capwap_control_ip_address.o 


LIBDIR		:= ../../../lib
LIBARCHDIR	:= $(LIBDIR)/$(ARCH)
OBJDIR		:= ../../../obj/mod_capwap/$(ARCH)
SNAME		:= $(LIBARCHDIR)/libmod_capwap.a
DNAME		:= $(LIBARCHDIR)/mod_capwap.so
MODNAME		:= $(LIBDIR)/mod_capwap.so
SLIBS		:= -lcw

include ../Mod.mak


