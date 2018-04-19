include ../Defs.mak
include ../CWConfig.mak
-include ../Config.local.mak
include ../Macros.mak


LIBDIR		:= ../../lib
LIBARCHDIR	:= $(LIBDIR)/$(ARCH)
OBJDIR		:= ../../obj/cw/$(ARCH)

SNAME		:= $(LIBARCHDIR)/libcw.a
DNAME		:= $(LIBARCHDIR)/libcw.so

LIBS=

ifeq ($(WITH_OPENSSL),1)
CFLAGS+=$(OPENSSL_CFLAGS)
CFLAGS+=-DWITH_OPENSSL
DTLSSRC += dtls_openssl.c \
	dtls_openssl_accept.c \
	dtls_openssl_connect.c \
	dtls_openssl_get_cipher.c \
	dtls_openssl_bio.c 
LIBS+=-lssl
endif

ifeq ($(WITH_GNUTLS),1)
CFLAGS+=$(GNUTLS_CFLAGS)
CFLAGS+=-DWITH_GNUTLS
DTLSSRC+= dtls_gnutls.c \
	dtls_gnutls_accept.c \
	dtls_gnutls_connect.c \
	dtls_gnutls_bio.c \
	dtls_gnutls_get_cipher.c \
	dtls_gnutls_get_peers_cert.c
LIBS+=-lgnutls
endif


CWSRC=\
	cw_check_missing_mand.c\
	cw_clock_lap.c\
	cw_dbg_elem.c\
	cw_dbg_set_level.c\
	cw_dbg_set_level_from_str.c\
	cw_filename.c\
	cw_format_dump.c\
	cw_format_pkt_hdr.c\
	cw_format_scan_hex_bytes.c\
	cw_format_version.c\
	cw_in_ac_name_with_priority.c\
	cw_in_capwap_local_ipv4_address.c\
	cw_in_capwap_local_ipv6_address.c\
	cw_in_check_cfg_update_req.c\
	cw_in_check_disc_req.c\
	cw_in_check_img_data_req_ac.c\
	cw_in_check_img_data_req_wtp.c\
	cw_in_check_join_req.c\
	cw_in_check_join_resp.c\
	cw_in_generic.c\
	cw_in_generic_struct.c\
	cw_in_radio_generic_struct.c\
	cw_in_idx_generic.c\
	cw_in_idx_generic_struct.c\
	cw_in_generic_enum.c\
	cw_out_generic_struct.c\
	cw_out_idx_generic_struct.c\
	cw_init_data_keep_alive_msg.c\
	cw_inline.c\
	cw_in_wtp_reboot_statistics.c\
	cw_is_printable.c\
	cw_load_file.c\
	cw_out_generic.c\
	cw_out_radio_administrative_states.c\
	cw_process_element.c\
	cw_put_elem_radio_administrative_state.c\
	cw_put_elem_radio_operational_state.c\
	cw_put_image_data.c\
	cw_put_local_ip_address.c\
	cw_put_msg.c\
	cw_radio_set_admin_state.c\
	cw_rand.c\
	cw_randint.c\
	cw_read_ac_descriptor.c\
	cw_read_descriptor_subelems.c\
	cw_readelem_statistics_timer.c\
	cw_read_wtp_descriptor_7.c\
	cw_read_wtp_descriptor.c\
	cw_result_is_ok.c\
	cw_save_file.c\
	cw_setstr.c\
	cw_strdup.c\
	cw_stricmp.c\
	cw_strlist_get_id.c\
	cw_strlist_get_str.c\
	cw_type_bstr16.c\
	cw_type_byte.c\
	cw_type_bool.c\
	cw_type_str.c\
	cw_type_dword.c\
	cw_type_ipaddress.c\
	cw_type_word.c\
	cw_type_sysptr.c\
	cw_write_descriptor_subelem.c\
	cw_read_from.c \
	cw_write_radio_element.c\
	cw_detect_nat.c\

KTVSRC=\
	cw_ktv_add.c\
	cw_ktv_replace.c\
	cw_ktv_add_from_str.c\
	cw_ktv_get_byte.c\
	cw_ktv_get_bool.c\
	cw_ktv_get_bstr16.c\
	cw_ktv_set_byte.c\
	cw_ktv_set_word.c\
	cw_ktv_set_dword.c\
	cw_ktv_get.c\
	cw_ktv_get_word.c\
	cw_ktv_get_dword.c\
	cw_ktv_get_sysptr.c\
	cw_ktv_get_str.c\
	cw_ktv_idx_get.c\
	cw_ktv_mavlcmp.c\
	cw_ktv_mavlcmp_type_by_name.c\
	cw_ktv_mavldel.c\
	cw_ktv_read_file.c\
	cw_ktv_readline.c\
	cw_ktv_read_struct.c\
	cw_ktv_write_struct.c\
	cw_ktv_std_types.c\
	cw_ktv_base_exists.c\
	cw_ktv_save.c\


LWSRC=\
	lw_addelem.c\
	lw_checksum.c\
	lw_elem_id_to_str.c\
	lw_inline.c\
	lw_msg_id_to_str.c\
	lwmsg_init.c\
	lw_put_80211_wtp_wlan_radio_configuration.c\
	lw_put_ac_descriptor.c\
	lw_put_bstr.c\
	lw_put_str.c\
	lw_put_image_data.c\
	lw_put_sockaddr.c\
	lw_readelem_wtp_name.c\

MAVLSRC=\
	mavl_add.c\
	mavl_add_ptr.c\
	mavl_cmpstr.c\
	mavl_create.c\
	mavl_del_all.c\
	mavl_del.c\
	mavl_destroy.c\
	mavl_foreach.c\
	mavl_foreach_lr.c\
	mavl_freeptr.c\
	mavl_get.c\
	mavl_get_ext.c\
	mavl_get_node.c\
	mavl_get_node_cmp..c\
	mavl_get_ptr.c\
	mavliter_get.c\
	mavliter_get_ptr.c\
	mavliter_init.c\
	mavliter_next.c\
	mavliter_seek.c\
	mavliter_seek_set.c\
	mavl_merge.c\
	mavl_replace.c\
	mavl_replace_data.c\


MLISTSRC=\
	mlist_append.c\
	mlist_delete.c\
	mlist_append_ptr.c\
	mlist_create.c\
	mlist_destroy.c\
	mlist_get.c\
	mlist_replace.c\

SOCKSRC=\
	sock_addrinit.c\
	sock_addrlen.c\
	sock_addrtostr.c\
	sock_cmpaddr.c\
	sock_copyaddr.c\
	sock_create.c\
	sock_getbroadcastaddr.c\
	sock_getifaddr.c\
	sock_getifhwaddr.c\
	sock_getifinfo.c\
	sock_getport.c\
	sock_get_primary_if.c\
	sock_hwaddrtostr.c\
	sock_mwait.c\
	sock_receive.c\
	sock_set_dontfrag.c\
	sock_setport.c\
	sock_set_recvtimeout.c\
	sock_strtoaddr.c\

LOGSRC=\
	log.c\
	log_file.c\
	log_errno.c\
	log_syslog.c\
	dbg.c\
	dbg_strings.c\

MISCSRC=\
	bstr16_create.c\
	bstr16_create_from_str.c\
	bstr_create.c\
	bstr_create_from_str.c\
	bstr_replace.c\
	bstrv_create.c\
	capwap_inline.c\
	capwap_strings_board.c\
	capwap_strings_elem80211.c\
	capwap_strings_elem.c\
	capwap_strings_msg.c\
	capwap_strings_result.c\
	capwap_strings_state.c\
	capwap_strings_vendor.c\
	conn_create.c\
	conn_create_noq.c\
	conn_destroy.c\
	conn_init.c\
	connlist.c\
	conn_prepare_image_data_request.c\
	conn_process_packet.c\
	conn_q_add_packet.c\
	conn_q_get_packet.c\
	conn_q_wait_packet.c\
	conn_q_recv_packet.c\
	conn_recv_packet.c\
	conn_send_data_packet.c\
	conn_send_msg.c\
	conn_send_packet.c\
	conn_send_request.c\
	dot11.c\
	dot11_inline.c\
	format.c\
	format_is_utf8.c\
	fragman.c\
	intavltree.c\
	md5sum.c\
	mod.c\
	msgset.c\
	netconn.c\
	send.c\
	strheap.c\

DTLSSRC+=\
	dtls_bio.c\
	cw_setup_dtls.c


RADIOSRC=\
	cw_out_radio_generic.c\
	cw_out_radio_generic_struct.c\
	cw_read_radio_generic.c\


#SRC=$(wildcard *.c)
SRC = $(CWSRC) $(LWSRC) $(MAVLSRC) $(MLISTSRC) $(SOCKSRC) $(LOGSRC) $(DTLSSRC) $(RADIOSRC) $(KTVSRC) $(MISCSRC)

OBJS=$(patsubst %.c,%.o,$(SRC))
OBJS:=$(patsubst %.o,$(OBJDIR)/%.o,$(OBJS))


all: $(SNAME) $(DNAME)


CFLAGS += $(GNUTLS_CFLAGS) \
	  -DWITH_CW_LOG \
	  -DWITH_CW_LOG_DEBUG \
	  -DWITH_DTLS 


$(OBJDIR)/%.o:%.c
	@mkdir -p $(OBJDIR)
	@echo "  $(CC) "$<
	@$(CC) -c $(CFLAGS) $< -o $@

$(SNAME) : $(OBJS) 
	@mkdir -p $(LIBARCHDIR)
	@echo "  $(AR) $(SNAME)"
	@$(AR) rcs $(SNAME) $(OBJS) 

$(DNAME) : $(OBJS) 
	@mkdir -p $(LIBARCHDIR)
	@echo "  $(LD) $(DNAME)"
	@$(LD) $(LDFLAGS) -shared -o $(DNAME) $(OBJS) $(LIBS)


SRCS = $(OBJS:.o=.c) 
DEPS := $(OBJS:.o=.d)


.PHONY: deps clean clean_libs libs



clean: 
	$(RM) $(OBJDIR)/*
	$(RM) $(DNAME)
	$(RM) $(SNAME)
	

clean_deps:
	$(DEPS) 
	
deps:  
	echo "# DO NOT DELETE THIS LINE -- make depend depends on it." > .depend
	makedepend $(SRC) -f.depend -p$(OBJDIR)/


-include .depend

