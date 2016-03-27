include ../CapwapConfig.mak
include ../Macros.mak

ifndef CC
	CC=gcc
endif

ifndef AR
	AR = ar
endif

#ifndef ARCH
#	ARCH = $(shell $(CC) -dumpmachine)
#endif



#OPENSSLINC=$(OPENSSLINC) 
#../contrib/openssl-1.0.1i/include

NAME=libcw.a
LIBDIR=../lib/$(ARCH)

SOCKOBJS=\
	sock_create.o\
	sock_copyaddr.o\
	sock_strtoaddr.o\
	sock_cmpaddr.o\
	sock_addrlen.o \
	sock_mwait.o\
	sock_addrtostr.o \
	sock_setport.o \
	sock_getport.o \
	sock_getifaddr.o \
	sock_getifhwaddr.o \
	sock_hwaddrtostr.o \
	sock_set_recvtimeout.o \
	sock_getbroadcastaddr.o \
	sock_addrinit.o \
	sock_set_dontfrag.o \
	sock_get_primary_if.o \
	sock_receive.o  \
	sock_getifinfo.o \


LOGOBJS=log.o \
        log_syslog.o \
	log_file.o \
	dbg_strings.o\
	dbg.o \


WTPINFOOBJS =\
	#wtpinfo_print.o 
	#wtpinfo_set_location.o \
	   wtpinfo_lwreadelem_wtp_descriptor.o \

UTILOBJS= \
	cw_setstr.o \
	cw_is_printable.o \
	cw_rand.o \
	stravltree.o \
	intavltree.o \
	cw_util.o \
	send.o \
	cw_read_wtp_descriptor.o \
	cw_read_wtp_descriptor_7.o \
	cw_read_wtp_descriptor_versions.o \
	cw_read_descriptor_subelems.o

MAVLOBJS= \
	mavl_del.o \
	mavl_add.o \
	mavl_create.o\
	mavl_get.o\
	mavl_del_all.o\
	mavliter_seek.o \
	mavliter_next.o \
	mavl_get_node.o \
	mavl_foreach.o \
	mavl_foreach_lr.o \
	mavl_merge.o \
	mavl_create_conststr.o \
	mlist.o \
	cw_load_file.o \
	cw_save_file.o


FORMATOBJS = \
	format_is_utf8.o \
	format_dot11_fc.o \



# LWAPP objs
LWAPPOBJS = \
	lw_checksum.o \
	lw_put_ac_descriptor.o \
	lw_put_cisco_path_mtu.o \
	lw_readelem_vendor_specific.o \
	lw_elem_id_to_str.o \
	lw_msg_id_to_str.o \
	lw_cisco_id_to_str.o \
	lw_put_sockaddr.o \
	lw_put_image_data.o \
	lw_in_vendor_specific.o


	#lw_put_80211_wtp_wlan_radio_configuration.o \
#	lw_readelem_wtp_name.o \
	lw_readelem_wtp_board_data.o \
	cw_log_debug.o \
	lw_vendor_id_to_str.o \


# LWAPP cisco vendor specific objs
LWAPPCISCOOBJS = \
	lw_addelem.o
 

# cw_tohex.o\
#capwap_parse_ctrlhdr.o \

#	   hdr_print.o \

CAPWAPOBJS= \
	   aciplist.o \
	   lwmsg_init.o \
	cw_cisco_id_to_str.o\
	cw_strlist_get_str.o \
	cw_strlist_get_id.o \
	capwap_strings_msg.o \
	capwap_strings_elem80211.o\
	acpriolist.o\
	capwap_strings_state.o \
	capwap_strings_vendor.o \
	capwap_strings_elem.o \
	cw_in_vendor_specific_payload.o \
	cw_in_mtu_discovery_padding.o\
	cw_in_wtp_board_data.o \
	cw_in_check_disc_req.o \
	cw_in_check_chng_state_evnt_req.o\
	cw_in_check_disc_resp.o\
	cw_in_check_cfg_update_req.o\
	cw_in_check_join_req.o \
	cw_in_check_cipwap_join_req.o \
	cw_in_check_join_resp.o \
	cw_in_check_img_data_req_ac.o \
	cw_in_check_img_data_req_wtp.o \
	cw_in_check_img_data_resp.o \
	cw_in_check_generic_resp.o \
	cw_in_check_generic_req.o \
	cw_out_generic.o \
	cw_out_wtp_descriptor.o \
	cw_out_wtp_reboot_statistics.o \
	cw_in_wtp_reboot_statistics.o\
	cw_in_cisco_image_identifier.o\
	cw_out_radio_operational_states.o\
	cw_out_radio_operational_states_7.o\
	cw_put_radio_operational_states.o \
	cw_put_ac_status.o \
	cw_in_ac_descriptor.o\
	cw_read_ac_descriptor.o\
	cw_out_capwap_local_ip_address.o\
	cw_out_wtp_ip_address.o\
	cw_out_capwap_control_ip_addr_list.o \
	cw_in_capwap_control_ip_address.o\
	cw_in_capwap_local_ipv4_address.o \
	cw_in_capwap_local_ipv6_address.o \
	cw_in_radio_administrative_state.o \
	cw_in_radio_administrative_state_wtp.o \
	cw_in_radio_operational_state.o\
	strheap.o \
	cw_check_missing_mand.o \
	md5sum.o \
	format.o \
	cw_in_cisco_add_wlan.o 

DOT11OBJS = \
	dot11.o \
	dot11_beacon.o

#	cw_out_ac_descriptor.o \



#
# SSL objects
#

ifeq ($(WITH_OPENSSL),1)
CFLAGS+=$(OPENSSL_CFLAGS)
CFLAGS+=-DWITH_OPENSSL
DTLSOBJS += dtls_openssl.o \
	dtls_openssl_accept.o \
	dtls_openssl_connect.o \
	dtls_openssl_get_cipher.o \
	dtls_openssl_bio.o 
endif

ifeq ($(WITH_GNUTLS),1)
CFLAGS+=$(GNUTLS_CFLAGS)
CFLAGS+=-DWITH_GNUTLS
DTLSOBJS+= dtls_gnutls.o \
	dtls_gnutls_accept.o \
	dtls_gnutls_connect.o \
	dtls_gnutls_bio.o \
	dtls_gnutls_get_cipher.o \
	dtls_gnutls_get_peers_cert.o
endif

DTLSOBJS+=dtls_bio.o 

CONNOBJS= conn_create.o \
	conn_process_packet.o \
	conn_q_add_packet.o \
	conn_q_get_packet.o \
	conn_q_recv_packet.o \
	conn_recv_packet.o \
	conn_destroy.o \
	connlist.o \
	conn_create_noq.o \
	conn_init.o \
	conn_send_packet.o \
	conn_send_data_packet.o

#	conn_send_msg.o
#conn_get_message.o  \
#	conn_send_response.o \
#	cw_prepare_configuration_status_request.o \
#	cw_prepare_change_state_event_request.o \
	conn_prepare_configuration_update_request.o \


	#bstr_to_str.o \

BSTROBJS= bstr_create.o \
	bstr_create_from_cfgstr.o \
	bstr16_create_from_cfgstr.o \
	bstr_replace.o \
	bstr16cfgstr.o \
	bstr16_create_from_str.o \
	bstr_create_from_str.o \
	bstrv_create_from_str.o 

	

FRAGOBJS=fragman.o

#	cw_in_wtp_descriptor.o \
#	capwap_80211_actions_wtp.o \
#	capwap_80211_actions_ac.o \
#	capwap_actions_ac.o \
	#capwap_actions_wtp.o \

CWACTION=action.o \
	cw_in_generic.o \
	cw_in_generic2.o \
	cw_out_wtp_board_data.o \
	cw_out_ac_name_with_priority.o \
	cw_in_ac_name_with_priority.o \
	cw_out_image_data.o \
	cipwap_actions_ac.o \
	cipwap_actions_wtp.o \
	cipwap_strings_elem.o \
	capwap_strings_result.o\
	capwap_strings_board.o \
	cw_put_msg.o \
	cw_put_mbag_item.o \
	cw_put_local_ip_address.o \
	capwap_action_helpers.o \
	cw_put_image_data.o \
	cisco.o \
	mbag.o\
	mbag_type_word.o\
	mbag_type_byte.o\
	mbag_type_vendorstr.o\
	mbag_type_sockaddr.o\
	mbag_type_dword.o\
	mbag_type_bstr16.o\
	mbag_type_mbag.o\
	mbag_set_from_buf.o\
	radio.o \
	cw_out_radio_info.o \
	capwap_items.o \
	item.o \
	mod.o

#	cw_process_msg.o \
#	item_strings.o \



OBJS=$(CONNOBJS) $(FRAGOBJS) $(SOCKOBJS) $(CAPWAPOBJS) $(WTPINFOOBJS) \
	$(LOGOBJS) $(UTILOBJS) $(DTLSOBJS) $(BSTROBJS) \
	$(LWAPPOBJS) \
	$(LWAPPCISCOOBJS) \
	$(CWACTION) \
	$(MAVLOBJS) \
	$(FORMATOBJS) \
	$(DOT11OBJS) 

#include $(OBJS:.o=.d)

O:=$(OBJS);

OBJS:=$(patsubst %.o,$(ARCH)/%.o,$(OBJS))





CFLAGS = -Wall -g -O0 -D_REENTRANT -DWITH_IPV6 -DWITH_RMAC_SUPPORT -I /usr/local/include -I../
#CFLAGS = -Wall  -Os -D_REENTRANT -DWITH_IPV6 -DWITH_RMAC_SUPPORT -I/usr/local/include

CFLAGS += $(GNUTLS_CFLAGS) \
	  -DWITH_CW_LOG \
	  -DWITH_CW_LOG_DEBUG \
	  -DWITH_DTLS \
	  $(XINCLUDE)\
	  -I $(OPENSSLINC)\
	-Werror 




#SRCS = $(OBJS:.o=.c) 
#MODOBJS=$(wildcard ./mod/cipwap/$(ARCH)/*.o)
#MODOBJS+=$(wildcard ./mod/capwap/$(ARCH)/*.o)

$(ARCH)/%.o:%.c
	@mkdir -p $(ARCH)
	@echo "  CC "$<
	@$(CC) -c $(CFLAGS) $< -o $@
#	@$(CC) -MM $(CFLAGS) $< > $*.d

$(LIBDIR)/$(NAME) : $(OBJS) $(MODOBJS)
	@mkdir -p $(LIBDIR)
	@echo "  AR $(LIBDIR)/$(NAME)"
	@$(AR) rcs $(LIBDIR)/$(NAME) $(OBJS) $(MODOBJS)


SRCS = $(OBJS:.o=.c) 
DEPS := $(OBJS:.o=.d)


.PHONY: deps clean clean_libs libs


# top-level rule, to compile everything. 
all: $(ARCH)/$(NAME)





clean: 
	$(RM) $(ARCH)/*
	$(RM) $(LIBDIR)/$(NAME)
#	$(RM) $(OBJS)
#	$(RM) $(DEPS)
#	$(RM) $(ARCH)/$(NAME) 
	

clean_deps:
	$(DEPS) 
	
deps:  
	$(CC) -MM -E *.c $(CFLAGS) > .depend

include .depend
