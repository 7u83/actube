#include "capwap_cisco.h"

/**
 * Add a Cisco AP Timesync message element to a buffer
 * @param dst destination buffer
 * @param time a unix timestamp
 * @param type of time
 * @return number of bytes put (5)
 */
int cw_put_cisco_ap_timesync(uint8_t * dst, time_t time, uint8_t type)
{
	cw_put_dword(dst , time);
	cw_put_byte(dst + 4, type);
	return 5;

}


int cw_addelem_cisco_ap_regulatory_domain(uint8_t *dst, struct radioinfo * ri){
	uint8_t *d=dst+10;
	
	d+=cw_put_byte(d,ri->rid);	/* Band ID */
	d+=cw_put_byte(d,1);		/* Set True/False */
	d+=cw_put_byte(d,ri->rid);	/* Slot ID */
	d+=cw_put_word(d,ri->regDomain);
	return 5 + cw_put_elem_vendor_hdr(dst, CW_VENDOR_ID_CISCO, CW_CISCO_AP_REGULATORY_DOMAIN, 5);
}


/** 
 * Add a Cisco Certificate payload message element
 * @param dst destination buffer
 * @param src pointer to DER certificate
 * @param len length of certificate
 * @return number of bytes put
 */
int cw_addelem_cisco_certificate(uint8_t*dst,uint8_t*src,int len){
	int l = lw_put_certificate(dst+10,src,len);
	return l+cw_put_elem_vendor_hdr(dst,CW_VENDOR_ID_CISCO,CW_CISCO_CERTIFICATE,l);
}


/*
int cw_addelem_cisco_wtp_radio_cfg(uint8_t * dst,struct radioinfo *ri){
	int l = lw_put_80211_wtp_wlan_radio_configuration(dst+10,ri);
	return l+cw_put_elem_vendor_hdr(dst,CW_VENDOR_ID_CISCO,CW_CISCO_STATION_CFG,l);
}
*/


int cw_readelem_cisco_station_cfg(uint8_t *src,int len){
	
//	lw_readelem_
	return 0;
}

