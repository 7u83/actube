#include <string.h>


#include "lwapp.h"
#include "lwapp_cisco.h"
#include "lw.h"
#include "vendors.h"



int lw_addelem(uint8_t*dst, uint8_t type, uint8_t *msgelem, uint16_t len)
{
	*dst = type;
	*((uint16_t*)(dst+1)) = htons(len);
	memcpy(dst,msgelem,len);
	return len+3;
}



int lw_addelem_vendor_specific(uint8_t *dst,uint32_t vendor_id,uint16_t elem_id, uint8_t *value, int len)
{
	lw_put_dword(dst+3,vendor_id);
	lw_put_word(dst+7,elem_id);
	memcpy(dst+9,value,len);
	int l = len+9;
	*dst=LW_ELEM_VENDOR_SPECIFIC;
	lw_put_word(dst+1,l);	
	return l;	
}

/**
 * Add the vendor specific elem Cisco Padding
 * @param dst destination
 */ 
int lw_addelem_cisco_padding(uint8_t *dst, int len)
{
	lw_put_dword(dst+3,LW_VENDOR_ID_CISCO);
	lw_put_word(dst+7,LW_CISCO_PATH_MTU);
	lw_put_word(dst+9,len);
	memset(dst+11,0,len);
	return lw_put_elem_hdr(dst,LW_ELEM_VENDOR_SPECIFIC,11+len);
}

/*
int lw_put_cisco_path_mtu(uint8_t *dst, uint16_t max, uint16_t padding)
{
	lw_put_dword(dst,LW_VENDOR_CISCO);
	lw_put_word(dst+4,LW_ELEM_CISCO_PATH_MTU);
	lw_put_word(dst+6,max);
	lw_put_word(dst+8,padding+4);
	memset(dst+10,0,padding);
	return padding+10;
}
*/


/*
int lw_addelem_cisco_path_mtu(uint8_t *dst,uint16_t max, uint16_t padding)
{
	lw_put_cisco_path_mtu(dst+3,max,padding);
}

*/

