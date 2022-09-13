#include "format.h"
#include "sock.h"
#include "conn.h"
#include "cw.h"

#include "dot11.h"

int cw_format_dot11_fc_flags(char *dst, uint8_t *frame){
	char *s = dst;
	uint8_t f = frame[0];
	s+=sprintf(s,"ToDS:%d ", f&1 ? 1:0);
	s+=sprintf(s,"FromDS:%d ", f&2 ? 1:0);
	s+=sprintf(s,"More Frgs:%d ", f&4 ? 1:0);
	s+=sprintf(s,"Retry:%d ", f&8 ? 1:0);
	s+=sprintf(s,"PwrMgmt:%d ", f&16 ? 1:0);
	s+=sprintf(s,"More Dta:%d ", f&32 ? 1:0);
	s+=sprintf(s,"Protec:%d ", f&64 ? 1:0);
	s+=sprintf(s,"+HTC/:%d ", f&128 ? 1:0);
	return s-dst;
}


int cw_format_dot11_rates(char *dst, const uint8_t *src, int len)
{
	int i;
	char * k="";
	char *s=dst;

	for(i=0; i<len; i++){
		int rate = src[i] &0x7f;
		s+=sprintf(s,"%s%0.1f",k,dot11_rate2float(rate));
		k=", ";
	}
	return s-dst;
}


int cw_format_dot11_elem(char *dst, uint8_t id, const uint8_t *src, int len)
{
	char *s = dst;
	s += sprintf(s,"elem(id=%d) ",id);


	switch(id){
		case DOT11_ELEM_SSID:
			s+=sprintf(s," ssid: %.*s",len,src);
			break;
		case DOT11_ELEM_SUPPORTED_RATES:
			s+=sprintf(s," Supported Rates:");
			s+=cw_format_dot11_rates(s,src,len);
			break;
		case DOT11_ELEM_EXTENDED_SUPPORTED_RATES:
			s+=sprintf(s," Extended Supported Rates");
			break;
		case DOT11_ELEM_POWER_CAPABILITY:
			s+=sprintf(s," Power Capability");
			break;
		case DOT11_ELEM_SUPPORTED_CHANNELS:
			s+=sprintf(s," Supported Channels");
			break;
		case DOT11_ELEM_SUPPORTED_OPERATING_CLASSES:
			s+=sprintf(s," Supported Operating Classes");
			break;
		case DOT11_ELEM_VENDOR_SPECIFIC:
			s+=sprintf(s," Vendor Specific");
			break;
	default:
			break;
	}
	s+=sprintf(s,", len=%d",len);
	return s-dst;
}


int cw_format_dot11_hdr(char * dst, uint8_t * packet, int len)
{
	char *s;
	int type = dot11_get_type_and_subtype(packet);

	s=dst;

	s+=sprintf(s,"IEEE 802.11 - %s",dot11_get_frame_name(packet));
	s+=sprintf(s," da:");
	s+=format_mac(s,dot11_get_da(packet),6);	
	s+=sprintf(s," sa:");
	s+=format_mac(s,dot11_get_sa(packet),6);	
	s+=sprintf(s," bssid:");
	s+=format_mac(s,dot11_get_bssid(packet),6);
	s+=sprintf(s," seq: %d\n",dot11_get_seq(packet));
	s+=cw_format_dot11_fc_flags(s,packet);
	s+=sprintf(s,"\nDuration: %d",dot11_get_duration(packet));
//
/*	switch (type){
		case DOT11_ASSOC_REQ:
			s+=sprintf(s,"\n   ssid: %.*s",dot11_assoc_req_get_ssid_len(packet),
					dot11_assoc_req_get_ssid(packet)
			);
			break;
			
	}*/

}




