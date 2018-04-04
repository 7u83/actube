#include "cw.h"

/** 
 * Get length wireless specific data
 * @param th Pointer to packet
 * @return length of wireless specific data
 *
 * Call this function only if the W flag is set
 */
int cw_get_hdr_ws_len(uint8_t * th)
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
uint8_t *cw_get_hdr_ws_data(uint8_t * th)
{
	if (!cw_get_hdr_flag_m(th))
		return th + 9;
	return (th + 10 + cw_get_hdr_rmac_len(th));
}



uint8_t * cw_get_hdr_payload_ptr(uint8_t *th)
{
	return th+cw_get_hdr_msg_offset(th);
}



/**
 * Set the HLEN field of a CAWAP Header
 * @param th pointer to the header
 * @param hlen value to set (Header Length)
 */
void cw_set_hdr_hlen(uint8_t * th, int hlen)
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
void cw_set_hdr_wbid(uint8_t * th, int wbid)
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
void cw_set_hdr_rid(uint8_t * th, int rid)
{
	uint32_t d = cw_get_dword(th);
	d &= (0x1f << 14) ^ 0xffffffff;
	d |= ((rid) & 0x1f) << 14;
	cw_set_dword(th, d);
}


uint8_t *cw_get_hdr_msg_elems_ptr(uint8_t * m)
{
	return cw_get_msg_elems_ptr(m + cw_get_hdr_msg_offset(m));
}

uint8_t *cw_get_hdr_msg_ptr(uint8_t * rawmsg)
{
	return rawmsg + cw_get_hdr_msg_offset(rawmsg);
}

int cw_get_hdr_msg_total_len(uint8_t * rawmsg)
{

	int offset = cw_get_hdr_msg_offset(rawmsg);
	return offset + cw_get_msg_elems_len(rawmsg + offset) + 8;
}



int cw_set_hdr_rmac(uint8_t * th, bstr_t rmac)
{
	int rmac_len;
	int hlen;
	
	if (!rmac) {
		cw_set_hdr_flags(th, CAPWAP_FLAG_HDR_M, 0);
		cw_set_hdr_hlen(th, 2);
		return 0;
	}
	rmac_len = bstr_len(rmac);
	memcpy(cw_get_hdr_rmac(th), rmac, rmac_len + 1);
	cw_set_hdr_flags(th, CAPWAP_FLAG_HDR_M, 1);

	hlen = 4 + rmac_len / 4;

	if (rmac_len % 4 != 0) {
		hlen++;
	}
	cw_set_hdr_hlen(th, hlen);
	return 1;
}


/** 
 * Put a message element header for a message to contain a vendor specific payload
 * @param dst pointer to destination buffer
 * @param vendorid vendorid
 * @param elemid element id of vendor specific data
 * @len length of vendor specific data 
 * @return the number of bytes put (always 10)
 */
int cw_put_elem_vendor_hdr(uint8_t * dst, uint32_t vendorid,
					 uint16_t elemid, uint16_t len)
{

	cw_set_elem_hdr(dst, CAPWAP_ELEM_VENDOR_SPECIFIC_PAYLOAD, len + 6);
	cw_set_dword(dst + 4, vendorid);
	cw_set_word(dst + 8, elemid);
	return 10;
}





/**
 * Add a message element to a buffer
 * @param dst pointer to buffer
 * @type message element type
 * @data pointer to data
 * @length of message element 
 * @return the number of bytes put
 */
int cw_addelem(uint8_t * dst, uint16_t type, uint8_t * data, uint16_t len)
{
	int l = cw_put_elem_hdr(dst, type, len);
	return l + cw_put_data(dst + l, data, len);
}


int cw_addelem_bstr(uint8_t * dst, uint16_t type, const bstr_t bstr)
{
	return cw_addelem(dst, type, bstr_data(bstr), bstr_len(bstr));
}


int cw_put_elem_result_code(uint8_t * dst, uint32_t code)
{
	cw_set_dword(dst + 4, code);
	return 4 + cw_put_elem_hdr(dst, CAPWAP_ELEM_RESULT_CODE, 4);
}





/*
int cw_put_version(uint8_t * dst, uint16_t subelem_id, bstrv_t v)
{
	uint8_t *d = dst;
	d += cw_put_dword(d, bstrv_get_vendor_id(v));
	d += cw_put_dword(d, (subelem_id << 16) | bstrv_len(v));
	d += cw_put_data(d, bstrv_data(v), bstrv_len(v));
	return d - dst;
}
*/


int cw_put_elem_radio_operational_state(uint8_t * dst, int rid, int state, int cause) {
	cw_set_byte(dst+4,rid);
	cw_set_byte(dst+5,state);
	cw_set_byte(dst+6,cause);
	return 3+cw_put_elem_hdr(dst,CW_ELEM_RADIO_OPERATIONAL_STATE,3);
}

int cw_put_elem_radio_administrative_state(uint8_t * dst, int rid, int state) {
	cw_set_byte(dst+4,rid);
	cw_set_byte(dst+5,state);
	return 2+cw_put_elem_hdr(dst,CW_ELEM_RADIO_ADMINISTRATIVE_STATE,2);
}


int cw_put_elem_session_id(uint8_t *dst, uint8_t *session_id, int len){
	memcpy(dst+4,session_id,len);
	return len+cw_put_elem_hdr(dst,CAPWAP_ELEM_SESSION_ID,len);
}


