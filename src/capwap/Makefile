include ../CapwapConfig.mak
include ../Macros.mak

ifndef CC
	CC=gcc
endif

ifndef AR
	AR = ar
endif

ifndef ARCH
	ARCH = $(shell $(CC) -dumpmachine)
endif



#OPENSSLINC=$(OPENSSLINC) 
#../contrib/openssl-1.0.1i/include

NAME=libcapwap.a

SOCKOBJS=sock_create.o sock_copyaddr.o sock_strtoaddr.o sock_cmpaddr.o sock_addrlen.o \
	sock_mwait.o sock_addrtostr.o \
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

LOGOBJS=log.o \
        log_syslog.o \
	log_file.o \
	cw_log_debug.o \
	cw_log_str2dbglevel.o \
	cw_dbg_elem.o




WTPINFOOBJS = wtpinfo_set_location.o \
	wtpinfo_print.o 

UTILOBJS= \
	cw_setstr.o \
	cw_is_printable.o \
	cw_rand.o \
	cw_foreach_msgelem.o \
	avltree.o \
	avltree_get_node.o \
	avltree_get.o \
	avltree_del_all.o \
	avltree_foreach_lr.o \
	avltree_foreach_rl.o \
	avltree_foreach_from_lr.o \
	avliter_next.o \
	avliter_seek.o \
	stravltree.o \
	intavltree.o \
	cw_util.o \
	cw_format_version.o \
	send.o



# LWAPP objs
LWAPPOBJS = \
	lw_checksum.o \
	lw_put_ac_descriptor.o \
	lw_put_cisco_path_mtu.o \
	lw_readelem_wtp_name.o \
	lw_put_80211_wtp_wlan_radio_configuration.o \
	lw_readelem_vendor_specific.o \
	lw_vendor_id_to_str.o \
	lw_elem_id_to_str.o \
	lw_msg_id_to_str.o \
	lw_cisco_id_to_str.o \
	lw_put_sockaddr.o \



# LWAPP cisco vendor specific objs
LWAPPCISCOOBJS = \
	lw_addelem.o
 

# cw_tohex.o\
#capwap_parse_ctrlhdr.o \


CAPWAPOBJS= \
	cwmsg_addelem_maximum_message_length.o \
	cwmsg_addelem_image_identifier.o \
	   cwmsg_send.o \
	   wtpinfo.o \
	   aciplist.o \
	   acinfo.o \
	   acinfo_print.o \
	   lwmsg_init.o \
	   wtpinfo_lwreadelem_wtp_descriptor.o \
	   hdr_print.o \
	cwmsg_addelem_vendor_cisco_mwar_addr.o \
	lw_readelem_wtp_board_data.o \
	cw_cisco_id_to_str.o\
	cw_strlist_get_str.o \
	capwap_strings_msg.o \
	capwap_strings_elem80211.o\
	acpriolist.o\
	capwap_strings_state.o \
	capwap_strings_vendor.o \
	capwap_strings_elem.o \
	itemstore.o \
	cw_in_vendor_specific_payload.o \
	cw_in_wtp_board_data.o \
	cw_in_check_disc_req.o \
	cw_in_check_disc_resp.o\
	cw_in_check_join_req.o \
	cw_in_check_join_resp.o \
	cw_in_check_img_data_req.o \
	cw_out_generic.o \
	cw_out_ac_descriptor.o \
	cw_out_cisco_ac_descriptor.o \
	cw_out_cisco_ap_timesync.o \
	cw_in_cisco_image_identifier.o\
	cw_out_capwap_control_ip_addr_list.o \
	cw_in_capwap_control_ipv4_address.o\
	strheap.o \
	cw_check_missing_mand.o \
	dbg.o \
	md5sum.o \
	format.o \
	
	
#		cwsend_unknow_response.o \
	cw_send_configuration_update_response.o \
	cwsend_echo_request.o \
		   cwmsg_init_echo_request.o \
	   cwmsg_addelem.o \
	   cwmsg_vaddelem.o \
	   cwmsg_addelem_ac_descriptor.o \
	   cwmsg_addelem_wtp_descriptor.o \
	   cwmsg_addelem_cw_local_ip_addr.o \
	   cwmsg_addelem_mtu_discovery_padding.o \
	   cwmsg_addelem_ac_timestamp.o \
cw_send_echo_response.o \
	cw_handle_echo_request.o \
	   cwsend_conf_status_response.o\
	cw_readmsg_configuration_status_response.o \
	cw_readmsg_configuration_status_request.o \
cw_readelem_maximum_message_length.o \
	cw_readelem_result_code.o\
	cw_readelem_ac_name.o \
	cw_readelem_ac_descriptor.o \
	cw_readelem_radio_administrative_state.o \
	cw_readelem_radio_operational_state.o \
	cw_readelem_statistics_timer.o \
	cw_readelem_mtu_discovery_padding.o \
	cw_readelem_vendor_specific_payload.o \
	cw_readelem_capwap_local_ip_addr.o \
	   cw_readelem_wtp_reboot_statistics.o\
	conn_get_response.o \
#	cwsend_change_state_event_response.o \
	   cwread_discovery_response.o \
	   cwsend_discovery_request.o \
	cw_send_image_file.o \
	   cwread_configuration_status_request.o\
	cwread_change_state_event_request.o\
	cwread_wtp_event_request.o \
	   process_conf_status_request.o \
	   cw_readelem_ecn_support.o \

#	   wtpinfo_readelem_wtp_mac_type.o \
#	   wtpinfo_readelem_wtp_descriptor.o \
#	   wtpinfo_readelem_discovery_type.o \
#	   wtpinfo_readelem_wtp_frame_tunnel_mode.o \
#	   wtpinfo_readelem_location_data.o \
#	   wtpinfo_readelem_wtp_name.o \
#	   wtpinfo_readelem_session_id.o \
#	   cwmsg_addelem_wtp_radio_infos.o \
#	cw_addelem_vendor_specific_payload.o \
	#cw_in_wtp_name.o \
	   #cw_msgtostr.o \
	   #cw_msgelemtostr.o \
	   #cwmsg_addelem_ctrl_ip_addrs.o \
	   #process_join_request.o \
	   #cwmsg_addelem_wtp_board_data.o \
	   #wtpinfo_readelem_wtp_board_data.o \
#	cw_send_image_data_response.o \
	   #cwmsg_init.o \
#	cw_readelem_80211_wtp_radio_info.o \
#	cwmsg_addelem_80211_add_wlan.o \
	   cwsend_discovery_response.o \
	cw_readelem_cisco_wtp_radio_cfg.o \
	cw_addelem_cisco_wtp_radio_cfg.o \
	
	   #cw_ianavendoridtostr.o \
	   #cwmsg_addelem_result_code.o \
#	cwmsg_addelem_radio_operational_state.o \
#	   wtpinfo_readelem_wtp_radio_info.o  \

#	cwmsg_addelem_vendor_cisco_ap_timesync.o \
#	cwmsg_addelem_vendor_specific_payload.o \
#	conn_prepare_image_data_request.o \

	#cwmsg_addelem_session_id.o
#	   process_msgelems_discovery_request.o \
	  # cwsend_join_request.o \
#	
	  # cwsend_image_data_request.o 
#	   cwmsg_set_control_header.o 
#	   process_msgelems.o \
#	   cwsend_join_response.o \
#	cwread_discovery_request.o\
	   cwread_join_response.o \
	cw_read_image_data_request.o \
#	conn_send_cwmsg.o \
#	conn_send_request.o \
#	conn_wait_for_request.o \

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
	conn_detect_capwap.o \
	conn_send_packet.o \
	conn_process_packet.o \
	conn_q_add_packet.o \
	conn_q_get_packet.o \
	conn_q_recv_packet.o \
	conn_recv_packet.o \
	conn_destroy.o \
	connlist.o \
	conn_create_noq.o \
	conn_send_response.o \
	conn_prepare_configuration_update_request.o \
	cw_prepare_configuration_status_request.o \
	cw_prepare_change_state_event_request.o \
	conn_prepare_request.o \
	conn_wait_for_message.o \
	conn_init.o \
	conn_send_msg.o
#conn_get_message.o  \



BSTROBJS= bstr_create.o \
	bstr_create_from_cfgstr.o \
	bstr_replace.o \
	bstr_to_str.o
	

FRAGOBJS=fragman.o

CWACTION=action.o \
	capwap_actions_ac.o \
	capwap_actions_wtp.o \
	capwap_80211_actions_wtp.o \
	cw_in_generic.o \
	cw_in_wtp_descriptor.o \
	cw_out_wtp_board_data.o \
	cipwap_actions_ac.o \
	cipwap_strings_elem.o \
	capwap_strings_result.o\
	cw_put_msg.o \
	capwap_action_helpers.o \

#	cw_process_msg.o \



OBJS=$(CONNOBJS) $(FRAGOBJS) $(SOCKOBJS) $(CAPWAPOBJS) $(WTPINFOOBJS) \
	$(LOGOBJS) $(UTILOBJS) $(DTLSOBJS) $(BSTROBJS) \
	$(LWAPPOBJS) \
	$(LWAPPCISCOOBJS) \
	$(CWACTION)

#include $(OBJS:.o=.d)

O:=$(OBJS);

OBJS:=$(patsubst %.o,$(ARCH)/%.o,$(OBJS))

CFLAGS = -Wall -g -O0 -D_REENTRANT -DWITH_IPV6 -DWITH_RMAC_SUPPORT -I /usr/local/include
CFLAGS = -Wall  -O3 -D_REENTRANT -DWITH_IPV6 -DWITH_RMAC_SUPPORT -I/usr/local/include

CFLAGS += $(GNUTLS_CFLAGS) \
	  -DWITH_CW_LOG \
	  -DWITH_CW_LOG_DEBUG \
	  -DWITH_DTLS \
	  $(XINCLUDE)\
	  -I $(OPENSSLINC)\
	-Werror 


#SRCS = $(OBJS:.o=.c) 

$(ARCH)/%.o:%.c
	@mkdir -p $(ARCH)
	@echo "  CC "$<
	@$(CC) -c $(CFLAGS) $< -o $@
#	@$(CC) -MM $(CFLAGS) $< > $*.d

$(ARCH)/$(NAME) : $(OBJS)
	@echo "  AR $(ARCH)/$(NAME)"
	@$(AR) rcs $(ARCH)/$(NAME) $(OBJS)


SRCS = $(OBJS:.o=.c) 
DEPS := $(OBJS:.o=.d)


.PHONY: deps clean clean_libs libs

# top-level rule, to compile everything. 
all: $(ARCH)/$(NAME) 


clean: 
	$(RM) $(ARCH)/*
#	$(RM) $(OBJS)
#	$(RM) $(DEPS)
#	$(RM) $(ARCH)/$(NAME) 
	

clean_deps:
	$(DEPS) 
	
deps: $(SRCS) 
	$(CC) -MD -E $(SRCS) $(CFLAGS) >/dev/null

-include $(DEPS)
