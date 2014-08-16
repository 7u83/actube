ifndef CC
	CC=gcc
endif

ifndef AR
	AR = ar
endif

ifndef ARCH
	ARCH = $(shell $(CC) -dumpmachine)
endif

ifdef INCLUDE_DIR
	XINCLUDE = -I $(INCLUDE_DIR)
endif

OPENSSLINC=../contrib/openssl-1.0.1i/include

NAME=libcapwap.a

SOCKOBJS=sock_create.o sock_copyaddr.o sock_strtoaddr.o sock_cmpaddr.o sock_addrlen.o \
	 sock_receive.o sock_mwait.o sock_addrtostr.o \
	 sock_setport.o \
	 sock_getport.o \
	 sock_getifaddr.o \
	 sock_getifhwaddr.o \
	 sock_hwaddrtostr.o \
	 sock_set_recvtimeout.o \
	 sock_getbroadcastaddr.o \
	 sock_addrinit.o \
	 sock_set_dontfrag.o \
	 sock_get_primary_if.o

LOGOBJS=cw_log.o \
	cw_log_debug.o \
	cw_log_tofile.o \
        cw_log_tosyslog.o 	


WTPINFOOBJS = wtpinfo_set_location.o \
	wtpinfo_print.o 

UTILOBJS=cw_setstr.o \
	 cw_is_printable.o \
	 cw_rand.o \
	 cw_pseudo_rand.o \
	 cw_foreach_msgelem.o \
	 avltree.o \
	 avltree_get.o \
	 avltree_del_all.o \
	 avltree_foreach.o \
	 stravltree.o

# cw_tohex.o\
#capwap_parse_ctrlhdr.o \

CAPWAPOBJS= \
	   cwmsg_init.o \
	   cwmsg_init_echo_request.o \
	   cwmsg_addelem.o \
	   cwmsg_addelem_ac_descriptor.o \
	   cwmsg_addelem_wtp_descriptor.o \
	   cwmsg_addelem_ctrl_ip_addrs.o \
	   cwmsg_addelem_wtp_radio_infos.o \
	   cwmsg_addelem_wtp_board_data.o \
	   cwmsg_addelem_cw_local_ip_addr.o \
	   cwmsg_addelem_mtu_discovery_padding.o \
	   cwmsg_addelem_result_code.o \
	   cwmsg_addelem_ac_timestamp.o \
	   cwmsg_addelem_vendor_specific_payload.o \
	   cwmsg_send.o \
	   cwsend_discovery_response.o \
	   cwread_discovery_response.o \
	   cwsend_discovery_request.o \
	   cwsend_join_request.o \
	   cwsend_join_response.o \
	   cwread_join_response.o \
	   cwsend_echo_request.o \
	   cwsend_echo_response.o \
	   cwsend_conf_status_response.o\
	   process_msgelems_discovery_request.o \
	   process_conf_status_request.o \
	   process_discovery_request.o \
	   process_join_request.o \
	   wtpinfo_readelem_wtp_board_data.o \
	   wtpinfo_readelem_wtp_mac_type.o \
	   wtpinfo_readelem_wtp_radio_info.o  \
	   wtpinfo_readelem_wtp_descriptor.o \
	   wtpinfo_readelem_discovery_type.o \
	   wtpinfo_readelem_wtp_frame_tunnel_mode.o \
	   wtpinfo_readelem_location_data.o \
	   wtpinfo_readelem_wtp_name.o \
	   wtpinfo_readelem_session_id.o \
	   wtpinfo.o \
	   aciplist.o \
	   acinfo.o \
	   acinfo_print.o \
	   lwmsg_init.o \
	   wtpinfo_lwreadelem_wtp_descriptor.o \
	   cw_msgelemtostr.o \
	   cw_msgtostr.o \
	   hdr_print.o \
	   cw_ianavendoridtostr.o \
	   cw_readelem_ecn_support.o \
	   cw_readelem_ac_name.o \
	   cw_readelem_wtp_reboot_statistics.o\
   	   cwmsg_addelem_vendor_cisco_ap_timesync.o
	

	
#	   cwmsg_set_control_header.o 
#	   process_msgelems.o \


DTLSOBJS= dtls_openssl.o	 \
	 dtls_openssl_accept.o \
	 dtls_openssl_connect.o \
	 dtls_openssl_get_cipher.o \
	 dtls_openssl_bio.o \
	 dtls_gnutls.o \
	 dtls_gnutls_accept.o \
	

CONNOBJS= conn.o \
	conn_send_packet.o \
	conn_send_cwmsg.o \
	conn_process_packet.o \
	conn_get_message.o  \
	conn_q_add_packet.o \
	conn_q_get_packet.o \
	conn_q_recv_packet.o \
	conn_recv_packet.o \
	conn_destroy.o \
	connlist.o \
	conn_create_noq.o \
	conn_send_response.o

	

FRAGOBJS=fragman.o

OBJS=$(CONNOBJS) $(FRAGOBJS) $(SOCKOBJS) $(CAPWAPOBJS) $(WTPINFOOBJS) $(LOGOBJS) $(UTILOBJS) $(DTLSOBJS)
O:=$(OBJS);

OBJS:=$(patsubst %.o,$(ARCH)/%.o,$(OBJS))

#CFLAGS = -Wall -g -O3 -D_REENTRANT -DWITH_IPV6
CFLAGS = -Wall -g -O0 -D_REENTRANT -DWITH_IPV6 -DWITH_RMAC_SUPPORT 

CFLAGS += -DWITH_CW_LOG \
	  -DWITH_CW_LOG_DEBUG \
	  -DWITH_DTLS \
	  -I $(OPENSSLINC)\
	  $(XINCLUDE)\
		-Werror 

#SRCS = $(OBJS:.o=.c) 

$(ARCH)/%.o:%.c
	@mkdir -p $(ARCH)
	@echo "  CC "$<
	@$(CC) -c $(CFLAGS) $< -o $@


$(ARCH)/$(NAME) : $(OBJS)
	@echo "  AR $(ARCH)/$(NAME)"
	@$(AR) rcs $(ARCH)/$(NAME) $(OBJS)
#$(OBJS)

#.c.o:
#	$(CC) -c $(CFLAGS) $<

SRCS = $(OBJS:.o=.c) 
DEPS := $(OBJS:.o=.d)


.PHONY: deps clean clean_libs libs

# top-level rule, to compile everything. 
all: $(ARCH)/$(NAME) 


clean: 
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	

clean_deps:
	$(DEPS) 
	
deps: $(SRCS) 
	$(CC) -MD -E $(SRCS) $(CFLAGS) >/dev/null

-include $(DEPS)
