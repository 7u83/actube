include ../../Defs.mak



OBJS=\
	mod_cisco_ac.o \
	cisco_actions_ac.o \
	cisco_in_wtp_descriptor.o \
	cisco_out_wtp_descriptor.o \
	cisco_in_ac_descriptor.o\
	cisco_out_ac_descriptor.o\
	cisco_out_radio_generic.o\
	cisco_out_ap_timesync.o\
	cisco_in_spam_vendor_specific.o\
	cisco_out_lw_path_mtu.o\
	cisco_out_capwap_local_ip_address.o


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
LIBARCHDIR	:= $(LIBDIR)/$(ARCH)
OBJDIR		:= ../../../obj/mod_cisco/$(ARCH)
SNAME		:= $(LIBARCHDIR)/libmod_cisco.a
DNAME		:= $(LIBARCHDIR)/mod_cisco.so
MODNAME		:= $(LIBDIR)/mod_cisco.so
SLIBS		:= -lcw -lmod_capwap -lconfuse


include ../Mod.mak


