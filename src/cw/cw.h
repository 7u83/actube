#ifndef __CW_H
#define __CW_H

#include <arpa/inet.h>

#include "bstr.h"
#include "mbag.h"
#include "conn.h"
#include "action.h"

#include "lw.h"
#include "capwap.h"

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





/** 
 * Macro to isolate bits from a dword 
 * @param src dword to isolate bits from
 * @param start start position
 * @len number of bits
 */
#define cw_get_dword_bits(src,start,len) ((~(0xFFFFFFFF<<len)) & (src >> (32 - start - len)))

/* macros to acces transport header values */

#define cw_get_hdr_preamble(th) (th[0])
#define cw_get_hdr_fragid(th) ((ntohl((((uint32_t*)th)[1]) >> 16) & 0xffff))
#define cw_get_hdr_fragoffset(th) ((ntohl((((uint32_t*)th)[1]) >> 3) & 0x1fff))
#define cw_get_hdr_rid(th) ((ntohl((((uint32_t*)th)[0]) >> 14) & 0x1f))
#define cw_get_hdr_wbid(th) ((ntohl(((uint32_t*)th)[0]) >> 9) & 0x1f)
#define cw_get_hdr_hlen(th) ((ntohl(((uint32_t*)th)[0]) >> 19) & 0x1f)

#define cw_get_hdr_rmac(th) (th+8)
#define cw_get_hdr_rmac_len(th) (*(th+8))
#define cw_get_hdr_rmac_size(th) cw_get_hdr_rmac_len(th)
#define cw_get_hdr_rmac_data(th) (th+9)

#define cw_get_hdr_flag_r1(th) ((ntohl( *((uint32_t*)th)) & CW_FLAG_HDR_R1 ) ? 1:0)
#define cw_get_hdr_flag_r2(th) ((ntohl( *((uint32_t*)th)) & CW_FLAG_HDR_R2 ) ? 1:0)
#define cw_get_hdr_flag_r3(th) ((ntohl( *((uint32_t*)th)) & CW_FLAG_HDR_R3 ) ? 1:0)

#define cw_get_hdr_flag_k(th) ((ntohl( *((uint32_t*)th)) & CW_FLAG_HDR_K ) ? 1:0)
#define cw_get_hdr_flag_m(th) ((ntohl( *((uint32_t*)th)) & CW_FLAG_HDR_M ) ? 1:0)
#define cw_get_hdr_flag_w(th) ((ntohl( *((uint32_t*)th)) & CW_FLAG_HDR_W ) ? 1:0)
#define cw_get_hdr_flag_l(th) ((ntohl( *((uint32_t*)th)) & CW_FLAG_HDR_L ) ? 1:0)
#define cw_get_hdr_flag_f(th) ((ntohl( *((uint32_t*)th)) & CW_FLAG_HDR_F ) ? 1:0)
#define cw_get_hdr_flag_t(th) ((ntohl( *((uint32_t*)th)) & CW_FLAG_HDR_T ) ? 1:0)

/** 
 * Get length wireless specific data
 * @param th Pointer to packet
 * @return length of wireless specific data
 *
 * Call this function only if the W flag is set
 */
static inline int cw_get_hdr_ws_len(uint8_t * th)
{
	if (!cw_get_hdr_flag_m(th)){
		return *(th + 8);
	}
	return *(th + 9 + cw_get_hdr_rmac_len(th));
}

/** 
 * Get pointer to wireless specific data
 * @param th Pointer to packet
 * @return Pointer to data
 *
 * Call this function only if the W flag is set
 */
static inline uint8_t *cw_get_hdr_ws_data(uint8_t * th)
{
	if (!cw_get_hdr_flag_m(th))
		return th + 9;
	return (th + 10 + cw_get_hdr_rmac_len(th));
}


#define cw_get_hdr_msg_offset(th) (4*cw_get_hdr_hlen(th))
#define cw_get_hdr_msg_elems_offset(th) (cw_get_hdr_msg_offset(th)+8)

#define cw_set_hdr_preamble(th,v) ((*th)=v)




/**
 * Set the HLEN field of a CAWAP Header
 * @param th pointer to the header
 * @param hlen value to set (Header Length)
 */
static inline void cw_set_hdr_hlen(uint8_t * th, int hlen)
{
	uint32_t d = cw_get_dword(th);
	d &= (0x1f << 19) ^ 0xffffffff;
	d |= ((hlen) & 0x1f) << 19;
	cw_set_dword(th, d);
}

/**
 * Set the WBID field of a CAWAP Header
 * @param th pointer to the header
 * @param wbid value to set (Wireless ID)
 */
static inline void cw_set_hdr_wbid(uint8_t * th, int wbid)
{
	uint32_t d = cw_get_dword(th);
	d &= (0x1f << 9) ^ 0xffffffff;
	d |= ((wbid) & 0x1f) << 9;
	cw_set_dword(th, d);
}

/**
 * Set the RID field of a CAWAP Header
 * @param th pointer to the header
 * @param rid value to set (Radio ID)
 */
static inline void cw_set_hdr_rid(uint8_t * th, int rid)
{
	uint32_t d = cw_get_dword(th);
	d &= (0x1f << 14) ^ 0xffffffff;
	d |= ((rid) & 0x1f) << 14;
	cw_set_dword(th, d);
}




/**
 * Set CAPWAP header flags
 * @param th pointer to header
 * @param flags list of flags to set or unset
 * @param set 1=set flag, 0=unset flag
 */
#define cw_set_hdr_flags(th,flags,set) \
	( set ? ((*((uint32_t*)th)) |= htonl(flags)) : ((*((uint32_t*)th)) &= (0xffffffff^htonl(flags))) )

#define cw_set_hdr_flag_f(th,set)  cw_set_hdr_flag(th, CW_FLAG_HDR_F)



/* Macros for message headers */

#define cw_get_msg_id(msgptr) (cw_get_dword(msgptr))
#define cw_get_msg_type(msgptr) cw_get_msg_id(msgptr)

#define cw_get_msg_seqnum(msgptr) cw_get_byte( (msgptr) +4 )
#define cw_get_msg_elems_len(msgptr) ( cw_get_word( (msgptr) +5 )-3)
#define cw_get_msg_elems_ptr(msgptr) ((msgptr)+8)


#define cw_set_msg_id(msgptr,t) cw_put_dword(msgptr,t)
#define cw_set_msg_type(msgptr,t) cw_set_msg_id(msgptr,t)
#define cw_set_msg_seqnum(msgptr,s) cw_put_byte( (msgptr) +4,s);
#define cw_set_msg_elems_len(msgptr,n) (cw_put_word((msgptr)+5,((n)+3)))

#define cw_set_msg_flags(msgptr,f) (cw_put_byte( (msgptr)+7,f))

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

#define cw_is_response(msg_id) (!is_response(msg_id))


static inline uint8_t *cw_get_hdr_msg_elems_ptr(uint8_t * m)
{
	return cw_get_msg_elems_ptr(m + cw_get_hdr_msg_offset(m));
}

static inline uint8_t *cw_get_hdr_msg_ptr(uint8_t * rawmsg)
{
	return rawmsg + cw_get_hdr_msg_offset(rawmsg);
}

#define cw_get_hdr_msg_id(ptr)\
	cw_get_msg_id(cw_get_hdr_msg_ptr(ptr))
#define cw_get_hdr_msg_type cw_get_hdr_msg_id

static inline int cw_get_hdr_msg_total_len(uint8_t * rawmsg)
{

	int offset = cw_get_hdr_msg_offset(rawmsg);
	return offset + cw_get_msg_elems_len(rawmsg + offset) + 8;
}



static inline int cw_set_hdr_rmac(uint8_t * th, bstr_t rmac)
{
	if (!rmac) {
		cw_set_hdr_flags(th, CW_FLAG_HDR_M, 0);
		cw_set_hdr_hlen(th, 2);
		return 0;
	}
	int rmac_len = bstr_len(rmac);
	memcpy(cw_get_hdr_rmac(th), rmac, rmac_len + 1);
	cw_set_hdr_flags(th, CW_FLAG_HDR_M, 1);

	int hlen = 4 + rmac_len / 4;

	if (rmac_len % 4 != 0) {
		hlen++;
	}
	cw_set_hdr_hlen(th, hlen);
	return 1;
}




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
	(cw_put_dword(dst, (((uint32_t)type)<<16) | (len)),4)


/** 
 * Put a message element header for a message to contain a vendor specific payload
 * @param dst pointer to destination buffer
 * @param vendorid vendorid
 * @param elemid element id of vendor specific data
 * @len length of vendor specific data 
 * @return the number of bytes put (always 10)
 */
static inline int cw_put_elem_vendor_hdr(uint8_t * dst, uint32_t vendorid,
					 uint16_t elemid, uint16_t len)
{

	cw_put_elem_hdr(dst, CW_ELEM_VENDOR_SPECIFIC_PAYLOAD, len + 6);
	cw_put_dword(dst + 4, vendorid);
	cw_put_word(dst + 8, elemid);
	return 10;
}




#define cw_put_sockaddr lw_put_sockaddr

int cw_put_image_data(uint8_t * dst, FILE * infile);

extern int cw_put_mbag_item(uint8_t * dst, struct mbag_item *item);


/**
 * Put encryption capabilities as described in CAWAP 
 * draft 7 fpr WTP Descriptor
 * @param dst Destination buffer
 * @param cap Capability
 * @return number of bytes put
 */
#define cw_put_encryption_capabilities_7(dst,cap) cw_put_word(dst,cap)


/**
 * Add a message element to a buffer
 * @param dst pointer to buffer
 * @type message element type
 * @data pointer to data
 * @length of message element 
 * @return the number of bytes put
 */
static inline int cw_addelem(uint8_t * dst, uint16_t type, uint8_t * data, uint16_t len)
{
	int l = cw_put_elem_hdr(dst, type, len);
	return l + cw_put_data(dst + l, data, len);
}


static inline int cw_addelem_bstr(uint8_t * dst, uint16_t type, const bstr_t bstr)
{
	return cw_addelem(dst, type, bstr_data(bstr), bstr_len(bstr));
}


static inline int cw_put_elem_result_code(uint8_t * dst, uint32_t code)
{
	cw_put_dword(dst + 4, code);
	return 4 + cw_put_elem_hdr(dst, CW_ELEM_RESULT_CODE, 4);
}






static inline int cw_put_version(uint8_t * dst, uint16_t subelem_id, bstrv_t v)
{
	uint8_t *d = dst;
	d += cw_put_dword(d, bstrv_get_vendor_id(v));
	d += cw_put_dword(d, (subelem_id << 16) | bstrv_len(v));
	d += cw_put_data(d, bstrv_data(v), bstrv_len(v));
	return d - dst;
}


extern int cw_put_ac_status(uint8_t * dst, struct cw_ac_status *s, struct conn *conn);


struct cw_descriptor_subelem_def {
	int vendor_id;
	int type;
	const char *item_id;
	int maxlen;
	int mand;
};


extern int cw_read_descriptor_subelems(mbag_t store, uint8_t * data, int len,
				       struct cw_descriptor_subelem_def *elems);

extern int cw_read_wtp_descriptor(mbag_t mbag, struct conn *conn,
				  struct cw_action_in *a, uint8_t * data, int len,
				  struct cw_descriptor_subelem_def *allowed);

extern int cw_read_wtp_descriptor_7(mbag_t mbag, struct conn *conn,
				    struct cw_action_in *a, uint8_t * data, int len,
				    struct cw_descriptor_subelem_def *allowed);

extern int cw_read_ac_descriptor(mbag_t store, uint8_t * data, int len,
				 struct cw_descriptor_subelem_def *allowed);

extern int cw_read_wtp_descriptor_versions(mbag_t mbag, uint8_t * data, int len);


extern int cw_in_check_generic_resp(struct conn *conn, struct cw_action_in *a,
				    uint8_t * data, int len, struct sockaddr *from);

extern int cw_in_check_generic_req(struct conn *conn, struct cw_action_in *a,
				   uint8_t * data, int len, struct sockaddr *from);

extern int cw_in_mtu_discovery_padding(struct conn *conn, struct cw_action_in *a,
				       uint8_t * data, int len, struct sockaddr *from);



/**
 * @defgroup CWELEMIN Input Handlers for Message Elements
 * @{
 */
extern int cw_in_wtp_reboot_statistics(struct conn *conn, struct cw_action_in *a,
				       uint8_t * data, int len, struct sockaddr *from);


extern int cw_in_wtp_board_data(struct conn *conn, struct cw_action_in *a, uint8_t * data,
				int len, struct sockaddr *from);

extern int cw_in_vendor_specific_payload(struct conn *conn, struct cw_action_in *a,
					 uint8_t * data, int len, struct sockaddr *from);

extern int cw_in_capwap_control_ip_address(struct conn *conn, struct cw_action_in *a,
					   uint8_t * data, int len,
					   struct sockaddr *from);

extern int cw_in_capwap_local_ipv4_address(struct conn *conn, struct cw_action_in *a,
					   uint8_t * data, int len,
					   struct sockaddr *from);

extern int cw_in_capwap_local_ipv6_address(struct conn *conn, struct cw_action_in *a,
					   uint8_t * data, int len,
					   struct sockaddr *from);

extern int cw_in_radio_operational_state(struct conn *conn, struct cw_action_in *a,
					 uint8_t * data, int len, struct sockaddr *from);





/**
 * @}
 */


/**
 * @defgroup CWELEMOUT Output Handlers for Message Elements
 * @{
 */
extern int cw_out_capwap_local_ip_address(struct conn *conn, struct cw_action_out *action,
					  uint8_t * dst);
extern int cw_out_wtp_ip_address(struct conn *conn, struct cw_action_out *action,
				 uint8_t * dst);

extern int cw_out_radio_operational_states(struct conn *conn, struct cw_action_out *a,
					   uint8_t * dst);
extern int cw_out_radio_operational_states_7(struct conn *conn, struct cw_action_out *a,
					     uint8_t * dst);


/**
 * With this alias for #cw_out_wtp_ip_address we can
 * can hav more consitent naming, when implement draft 7 
 * specific stuff 
 */
#define cw_out_capwap_local_ip_address_7 cw_out_wtp_ip_address

extern int cw_out_wtp_reboot_statistics(struct conn *conn, struct cw_action_out *a,
					uint8_t * dst);

/* helpers */
extern int cw_put_local_ip_address(int sock, uint8_t * dst, int ipv4elem_id,
				   int ipv6elem_id);
extern int cw_put_radio_operational_states(mbag_t radios, uint8_t * dst, int *nerror,
					   int d7mode);


/**
 * @}
 */


/**
 * @defgroup CAPWAP_RADIO Radio Operations
 * @{
 */
extern int cw_radio_set_admin_state(mbag_t radios,int rid, int state, int cause);
extern int cw_put_elem_radio_administrative_state(uint8_t *dst,int radio_id,mbag_t radio);
extern int cw_put_elem_radio_operational_state(uint8_t * dst, int rid, int os, int d7mode);



/**
 * @}
 */



/**
 *@}
 */

#endif
