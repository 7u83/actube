#include "cw/sock.h"
#include "cw/lwapp_cisco.h"
#include "cw/capwap_cisco.h"
#include "cw/action.h"
#include "cw/vendors.h"
#include "cw/lw.h"
#include "cw/cw.h"






int cisco_out_manager_ip_addr(struct conn *conn,struct cw_action_out * a,uint8_t *dst) 
{
uint8_t arr[]= {
0x00,0x40,0x96,0x00,0x00,0x68,0x00,0x40,0x96,0x00,0x00,0x87,0xC0,0xA8,0x00,0x5A,0x08,0x00,0x27,0x01,0xA0,0x83

};

uint8_t arr1[] = {
	0xC0,0xA8,0x00,0x0B,0x08,0x00,0x27,0x01,0xA0,0x83
};
	memcpy(dst+16,arr1,sizeof(arr1));

	int l=sizeof(arr1);
	l = l + lw_put_vendor(dst+10, LW_VENDOR_ID_CISCO, LW_CISCO_DOT11R_WLC_MAC_AND_IP, l);
	return l+cw_put_elem_vendor_hdr(dst,CW_VENDOR_ID_CISCO,CW_CISCO_SPAM_VENDOR_SPECIFIC,l);



	uint8_t *d=dst+16;

	struct sockaddr ss;

	sock_strtoaddr("192.168.0.11",&ss);
	memcpy(d,&(((struct sockaddr_in *) &ss)->sin_addr),4);
	 l=4;
	l = l + lw_put_vendor(dst+10, LW_VENDOR_ID_CISCO, LW_CISCO_MANAGER_IP_ADDR, l);
	return l+cw_put_elem_vendor_hdr(dst,CW_VENDOR_ID_CISCO,CW_CISCO_SPAM_VENDOR_SPECIFIC,l);
}

