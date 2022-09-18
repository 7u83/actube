#ifndef __CW_H
#define __CW_H

#include <arpa/inet.h>

/*#include "bstr.h"*/

/*#include "conn.h"*/


#include "lw.h"
#include "capwap.h"
#include "msgset.h"

/**
 * @addtogroup CAPWAP
 * @{
 */

/**
 * @defgroup CW CW
 * @{
 */


/* Use some macros from LWAPP */

/**
 * Put a byte to an output buffer. 
 * see #lw_put_byte 
 */
#define cw_put_byte lw_put_byte

/**
 * Put a word to an output buffer.
 * see #lw_put_word
 */
#define cw_put_word lw_put_word

/**
 * Put a dword 
 * see #lw_put_dword
 */
#define cw_put_dword lw_put_dword

/**
 * Put some bytes
 * See #lw_put_data
 */
#define cw_put_data lw_put_data


/**
 * Put a 0-terminated string
 */
#define cw_put_str lw_put_str

/**
 * Put a bstr_t object
 * see #lw_put_bstr
 */
#define cw_put_bstr lw_put_bstr

/**
 * Put bstr16_t object
 * see #lw_put_bstr16
 */
#define cw_put_bstr16 lw_put_bstr16

/**
 * Set dword
 * see #lw_set_dword
 */
#define cw_set_dword lw_set_dword

#define cw_get_byte lw_get_byte
#define cw_get_word lw_get_word
#define cw_get_dword lw_get_dword


#define cw_set_byte lw_set_byte
#define cw_set_word lw_set_word
#define cw_set_dword lw_set_dword


/*
#define cw_get_str(data, maxlen) (strndup((char*)(data),maxlen))
*/

/** 
 * Macro to isolate bits from a dword 
 * @param src dword to isolate bits from
 * @param start start position
 * @len number of bits
 */
#define cw_get_dword_bits(src,start,len) ((~(0xFFFFFFFF<<len)) & (src >> (32 - start - len)))

/* macros to acces transport header values */

#define cw_get_hdr_preamble(th) (th[0])

#define cw_get_hdr_fragid(th)\
	(((ntohl(*(((uint32_t*)(th))+1))) >> 16) & 0xffff)

#define cw_get_hdr_fragoffset(th)\
	(((ntohl(*(((uint32_t*)(th))+1))) >> 3) & 0x1fff)

#define cw_get_hdr_rid(th) ((ntohl((((uint32_t*)th)[0]) >> 14) & 0x1f))
#define cw_get_hdr_wbid(th) ((ntohl(((uint32_t*)th)[0]) >> 9) & 0x1f)
#define cw_get_hdr_hlen(th) ((ntohl(((uint32_t*)th)[0]) >> 19) & 0x1f)

#define cw_get_hdr_rmac(th) (th+8)
#define cw_get_hdr_rmac_len(th) (*(th+8))
#define cw_get_hdr_rmac_size(th) cw_get_hdr_rmac_len(th)
#define cw_get_hdr_rmac_data(th) (th+9)

#define cw_get_hdr_flag_r1(th) ((ntohl( *((uint32_t*)th)) & CAPWAP_FLAG_HDR_R1 ) ? 1:0)
#define cw_get_hdr_flag_r2(th) ((ntohl( *((uint32_t*)th)) & CAPWAP_FLAG_HDR_R2 ) ? 1:0)
#define cw_get_hdr_flag_r3(th) ((ntohl( *((uint32_t*)th)) & CAPWAP_FLAG_HDR_R3 ) ? 1:0)

#define cw_get_hdr_flag_k(th) ((ntohl( *((uint32_t*)th)) & CAPWAP_FLAG_HDR_K ) ? 1:0)
#define cw_get_hdr_flag_m(th) ((ntohl( *((uint32_t*)th)) & CAPWAP_FLAG_HDR_M ) ? 1:0)
#define cw_get_hdr_flag_w(th) ((ntohl( *((uint32_t*)th)) & CAPWAP_FLAG_HDR_W ) ? 1:0)
#define cw_get_hdr_flag_l(th) ((ntohl( *((uint32_t*)th)) & CAPWAP_FLAG_HDR_L ) ? 1:0)
#define cw_get_hdr_flag_f(th) ((ntohl( *((uint32_t*)th)) & CAPWAP_FLAG_HDR_F ) ? 1:0)
#define cw_get_hdr_flag_t(th) ((ntohl( *((uint32_t*)th)) & CAPWAP_FLAG_HDR_T ) ? 1:0)










/*
#define CW_RECEIVER_AC 1
#define CW_RECEIVER_WTP 1
*/

#define	CW_ROLE_AC	1
#define CW_ROLE_WTP	2
#define CW_ROLE_BOTH	(CW_ROLE_AC | CW_ROLE_WTP)



int cw_get_hdr_ws_len(uint8_t * th);
int cw_get_hdr_ws_len_7(uint8_t * th);

uint8_t *cw_get_hdr_ws_data(uint8_t * th);
#define cw_get_hdr_ws_data_7(th) (cw_get_hdr_ws_data(th)+1)


#define cw_get_hdr_msg_offset(th) (4*cw_get_hdr_hlen(th))

uint8_t * cw_get_hdr_payload_ptr(uint8_t *th);

#define cw_get_hdr_msg_elems_offset(th) (cw_get_hdr_msg_offset(th)+8)

#define cw_set_hdr_preamble(th,v) ((*th)=v)



void cw_set_hdr_hlen(uint8_t * th, int hlen);
void cw_set_hdr_wbid(uint8_t * th, int wbid);
void cw_set_hdr_rid(uint8_t * th, int rid);



/**
 * Set CAPWAP header flags
 * @param th pointer to header
 * @param flags list of flags to set or unset
 * @param set 1=set flag, 0=unset flag
 */
#define cw_set_hdr_flags(th,flags,set) \
	( set ? ((*((uint32_t*)th)) |= htonl(flags)) : ((*((uint32_t*)th)) &= (0xffffffff^htonl(flags))) )

#define cw_set_hdr_flag_f(th,set)  cw_set_hdr_flag(th, CAPWAP_FLAG_HDR_F)



/* Macros for message headers */

#define cw_get_msg_id(msgptr) (cw_get_dword(msgptr))
#define cw_get_msg_type(msgptr) cw_get_msg_id(msgptr)

#define cw_get_msg_seqnum(msgptr) cw_get_byte( (msgptr) +4 )
#define cw_get_msg_elems_len(msgptr) ( cw_get_word( (msgptr) +5 )-3)
#define cw_get_msg_elems_ptr(msgptr) ((msgptr)+8)

#define cw_get_datamsg_elems_len(msgptr) (cw_get_word(msgptr));
#define cw_get_datamsg_elems_ptr(msgptr) ((msgptr)+2);

/**
 * Set the elems_len field of a data (keep-alive) msg
 * @param msgptr Pointer to payload
 * @param lem Value to set
 */
#define cw_set_datamsg_elems_len(msgptr,len) (cw_put_word(msgptr,len));

#define cw_set_msg_id(msgptr,t) cw_set_dword(msgptr,t)
#define cw_set_msg_type(msgptr,t) cw_set_msg_id(msgptr,t)
#define cw_set_msg_seqnum(msgptr,s) cw_set_byte( (msgptr) +4,s);
#define cw_set_msg_elems_len(msgptr,n) (cw_set_word((msgptr)+5,((n)+3)))

#define cw_set_msg_flags(msgptr,f) (cw_set_byte( (msgptr)+7,f))

/**
 * Check if a message id is from request msg 
 * @param msg_id id to check
 * @return 1 if it msg id is a request msg id, otherwise 0
 */
#define cw_is_request(msg_id) (msg_id & 1)

/**
 * Check if message id is from a response msg
 * @see #cw_is_request
 */

#define cw_is_response(msg_id) (!cw_is_resquest(msg_id))


uint8_t *cw_get_hdr_msg_elems_ptr(uint8_t * m);
uint8_t *cw_get_hdr_msg_ptr(uint8_t * rawmsg);



#define cw_get_hdr_msg_id(ptr)\
	cw_get_msg_id(cw_get_hdr_msg_ptr(ptr))
#define cw_get_hdr_msg_type cw_get_hdr_msg_id

int cw_get_hdr_msg_total_len(uint8_t * rawmsg);
int cw_set_hdr_rmac(uint8_t * th, bstr_t rmac);



/**
 * Get length of a CAPWAP message elemet 
 * @param e pointer to element (uint8_t*)
 * @return length of element
 */
#define cw_get_elem_type(e) cw_get_word(e)

/**
 * Alias for #cw_get_elem_type
 */
#define cw_get_elem_id(e) cw_get_elem_type(e)

/**
 * Get type of a CAPWAP message element
 * @pram e pointer to element (uint8_t*)
 * @return type of element
 */
#define cw_get_elem_len(e) cw_get_word((e)+2)

/**
 * Get a pointer to the data of a CAPWAP message element 
 * @param e pointer to message element 
 * @return pointer to data
 */
#define cw_get_elem_data(e) ((e)+4)

/** 
 * Iterate through message elements of a CAPWAP message
 * @param i iterator witch points to the current element (uint8_t*)
 * @param elems pointer to message elements (uint8_t*)
 * @param len length of message element buffer
 * 
 * You can use this macro like a for loop.
 * 
 * uint8_t * i
 * cw_foreach_elem(i,elem,len){
 *   ...
 *   print_message(i);
 *   ...
 * }
 */
#define cw_foreach_elem(i,elems,len) for(i=elems; i<elems+len; i=i+4+cw_get_elem_len(i))


/**
 * Put a message element header to buffer
 * @param dst pointer to buffer (uint8_t)
 * @param type tpe of message element
 * @param len length of message element data
 * @return the number bytes put (always 4)
 */

#define cw_put_elem_hdr(dst,type,len) \
	(cw_set_dword(dst, (((uint32_t)type)<<16) | (len)),4)

/**
 * same as #cw_put_elem_hdrm but without return value
 */
#define cw_set_elem_hdr(dst,type,len) \
	(cw_set_dword(dst, (((uint32_t)type)<<16) | (len)))

int cw_put_elem_vendor_hdr(uint8_t * dst, uint32_t vendorid,
					 uint16_t elemid, uint16_t len);



#define cw_put_sockaddr lw_put_sockaddr

int cw_put_image_data(uint8_t * dst, FILE * infile);




/**
 * Put encryption capabilities as described in CAWAP 
 * draft 7 fpr WTP DescriptorSubelemDef
 * @param dst Destination buffer
 * @param cap Capability
 * @return number of bytes put
 */
#define cw_put_encryption_capabilities_7(dst,cap) cw_put_word(dst,cap)

/*
int cw_addelem(uint8_t * dst, uint16_t type, uint8_t * data, uint16_t len);
int cw_addelem_bstr(uint8_t * dst, uint16_t type, const bstr_t bstr);
*/
int cw_put_elem_result_code(uint8_t * dst, uint32_t code);






/*extern int cw_put_ac_status(uint8_t * dst, struct cw_ac_status *s, struct cw_Conn *conn);*/


struct cw_DescriptorSubelemDef {
	int vendor_id;
	int type;
	const char *key;
	int maxlen;
	int mand;
};

#define CW_DELETE 1
#define CW_APPEND 2
#define CW_PREPEND 3
#define CW_REPLACE 4
#define CW_IGNORE 5





//int cw_check_missing_mand(struct cw_MsgData *msgdata, mavl_t keys );
int cw_check_missing_mand(struct cw_MsgData *msgdata, mavl_t keys, mavl_t handlers_by_key );



extern int cw_read_descriptor_subelems(cw_Cfg_t * store, const char * key, uint8_t * data, int len,
				       struct cw_DescriptorSubelemDef *elems);

/*
int cw_read_wtp_descriptor(mavl_t mbag, struct cw_Conn *conn,
				  struct cw_ElemHandler *eh, uint8_t * data, int len,
				  struct cw_DescriptorSubelemDef *allowed);
*/

int cw_read_wtp_descriptor(cw_Cfg_t * mbag, struct cw_Conn *conn,
			   struct cw_ElemHandler *eh, uint8_t * data, int len,
			   struct cw_DescriptorSubelemDef *allowed);

int cw_write_descriptor_subelem (uint8_t *dst, cw_Cfg_t ** cfg_list,
                                 int subelem_id, const char * parent_key);

int cw_write_radio_element(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params, int idx,
			 uint8_t * dst);
			

extern int cw_read_wtp_descriptor_7(cw_Cfg_t * cfg, struct cw_Conn *conn,
				    struct cw_ElemHandler *eh, uint8_t * data, int len,
				    struct cw_DescriptorSubelemDef *allowed);


int cw_read_ac_descriptor(cw_Cfg_t * store,
			   struct cw_ElemHandler *eh,
			   struct cw_ElemHandlerParams * params,
			   uint8_t *data, int len, 
			struct cw_DescriptorSubelemDef *allowed);


int cw_setup_dtls(struct cw_Conn * conn, cw_Cfg_t * cfg, const char *prefix, char  * default_cipher);




/**
 * @defgroup CWELEMIN Input Handlers for Message Elements
 * @{
 */
 
 
 
int cw_in_idx_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len);
		
int cw_in_generic_with_index(struct cw_ElemHandler *eh, 
		struct cw_ElemHandlerParams *params, 
			uint8_t * data,	 int len);
int cw_out_generic_with_index(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst);
		
int cw_in_idx_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len);

int cw_out_idx_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst);

int cw_in_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len);

int cw_in_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len);

int cw_in_generic_enum(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len);
		
int cw_in_generic_indexed_enum(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len);

int cw_out_generic_indexed_enum(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst);

int cw_out_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst);

int cw_out_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst);

int cw_in_radio_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len);

int cw_in_radio_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params,
		uint8_t * elem_data, int elem_len);

int cw_out_radio_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst);
			
int cw_out_radio_generic_struct(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst);

extern int cw_in_wtp_reboot_statistics(struct cw_Conn *conn, struct cw_action_in *a,
				       uint8_t * data, int len, struct sockaddr *from);


extern int cw_in_wtp_board_data(struct cw_Conn *conn, struct cw_action_in *a, uint8_t * data,
				int len, struct sockaddr *from);

/*					 
int cw_in_vendor_specific_payload(struct cw_ElemHandler *handler, 
				struct cw_ElemHandlerParams *params,
				  uint8_t * data, int len);
*/

extern int cw_in_capwap_control_ip_address(struct cw_Conn *conn, struct cw_action_in *a,
					   uint8_t * data, int len,
					   struct sockaddr *from);

extern int cw_in_capwap_local_ipv4_address(struct cw_Conn *conn, struct cw_action_in *a,
					   uint8_t * data, int len,
					   struct sockaddr *from);

extern int cw_in_capwap_local_ipv6_address(struct cw_Conn *conn, struct cw_action_in *a,
					   uint8_t * data, int len,
					   struct sockaddr *from);

extern int cw_in_radio_operational_state(struct cw_Conn *conn, struct cw_action_in *a,
					 uint8_t * data, int len, struct sockaddr *from);


/*	
int cw_process_element(struct cw_Conn * conn, 
		struct cw_MsgData * msgdata, int proto, int vendor,
		uint8_t  * elem, int max_len);
*/
int cw_process_element(struct cw_ElemHandlerParams *params, 
		int proto, int vendor,int msgid,
		uint8_t  * elem, int max_len);

/**
 * @}
 */


/**
 * @defgroup CWELEMOUT Output Handlers for Message Elements
 * @{
 */



/**
 * With this alias for #cw_out_wtp_ip_address we can
 * can hav more consitent naming, when implement draft 7 
 * specific stuff 
 */
#define cw_out_capwap_local_ip_address_7 cw_out_wtp_ip_address


void cw_init_request(struct cw_Conn *conn, int msg_id);


/**
 * @}
 */


/**
 * @defgroup CAPWAP_RADIO Radio Operations
 * @{
 */
/* 
extern int cw_radio_set_admin_state(mbag_t radios,int rid, int state, int cause);
 */
 

int cw_put_elem_radio_operational_state(uint8_t * dst, int rid, int state, int cause);
int cw_put_elem_radio_administrative_state(uint8_t * dst, int rid, int state);



int cw_put_local_ip_address(uint8_t *dst, int id, int ipv_id, int ipv6_id, 
				uint8_t *src, int len);

int cw_detect_nat(struct cw_ElemHandlerParams  *conn);

uint8_t *cw_init_data_keep_alive_msg(uint8_t * buffer,uint8_t *rmac);

/*int cw_out_radio_generic(struct cw_Conn *conn, struct cw_action_out *a, uint8_t * dst);*/

int cw_put_elem_session_id(uint8_t *dst, uint8_t *session_id, int len);

int cw_result_is_ok( int rc );
int cw_assemble_message(struct cw_Conn *conn, uint8_t * rawout);

/**
 * @}
 */



char *cw_strdup(const char *s);
int cw_stricmp(char const *a, char const *b);

int cw_write_header(struct cw_ElemHandler * handler, uint8_t * dst, int len);
int cw_header_len(struct cw_ElemHandler * handler);

int cw_compose_message(struct cw_Conn *conn, uint8_t * rawout);
int cw_put_ac_status(uint8_t *dst, cw_Cfg_t ** cfg_list, const char * parent_key);
int cw_put_descriptor_subelem (uint8_t *dst, cw_Cfg_t ** cfg_list,
                                 int subelem_id, const char * parent_key );


int cw_send_request(struct cw_Conn *conn,int msg_id);
int cw_out_generic_walk(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst);
int cw_out_generic0(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst,const char *key);

/**
 *@}
 */


/**
 *@} CAPWAP
 */

#endif
