include ../../Defs.mak



SOURCES=\
	mod_cisco_ac.c \
	cisco_actions.c \
	cisco_in_wtp_descriptor.c \
	cisco_out_wtp_descriptor.c \
	cisco_in_ac_descriptor.c\
	cisco_out_ac_descriptor.c\
	cisco_out_radio_generic.c\
	cisco_out_ap_timesync.c\
	cisco_in_spam_vendor_specific.c\
	cisco_out_lw_path_mtu.c\
	cisco_out_capwap_local_ip_address.c


#	cisco80211.o \
#	cisco_out_radio_administrative_state.o \
	cisco_in_radio_administrative_state.o \
	cisco_in_spam_vendor_specific.o \
#	cisco_out_ap_timesync.o \
	cisco_out_board_data_options.o \
#	cisco_out_ac_descriptor.o \
#	cisco_out_telnet_ssh.o \
	cisco_in_telnet_ssh.o \
	cisco80211_in_mac_operation.o \
	cisco80211_in_wtp_radio_configuration.o \
	cisco80211_out_wtp_radio_configuration.o \
	cisco_out_manager_ip_addr.o \
	cisco_out_ac_ipv4_list.o \
	cisco_out_capwap_up.o \
	cisco_in_add_wlan.o \
	cisco_out_wtp_administrative_state.o \
	cisco_out_radio_operational_state.o \
	cisco_in_radio_operational_state.o \
	cisco_out_wtp_operational_state.o

	
LIBDIR		:= ../../../lib
LIBARCHDIR	:= $(LIBDIR)/$(KERNEL)/$(ARCH)
OBJDIR		:= ../../../obj/mod_cisco/$(KERNEL)/$(ARCH)
SNAME		:= $(LIBARCHDIR)/libmod_cisco.a
DNAME		:= $(LIBARCHDIR)/mod_cisco.so

CFLAGS+=-fPIC
CFLAGS+= -I ../../ -I../ -I../../include -I../../../include -I/usr/local/include
CFLAGS+= -DUSE_OPENSSL

all: $(SNAME) $(DNAME) 
static: $(SNAME)
dynamic: $(DNAME)

include ../../lib.mk


