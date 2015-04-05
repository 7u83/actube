
#include <string.h>

#include "capwap.h"
#include "bstr.h"


static inline int wtpdesc_addsubelem(uint8_t * dst,uint8_t type,uint32_t vendorid,bstr_t str)  
{
	int l;
	*((uint32_t*)(dst))=htonl(vendorid);   
	l = bstr_len(str);
	*((uint32_t*)(dst+4))=htonl((type<<16)|l);
	memcpy(dst+8,bstr_data(str),l);
	return l+8;
}

void cwmsg_addelem_wtp_descriptor(struct cwmsg * cwmsg, struct wtpinfo * wtpinfo)
{
	uint8_t d[1024];
	int len=0;

	/* radios info */
	*(d)= wtpinfo->max_radios;
        *(d+1)=wtpinfo->radios_in_use;
	len=2;

	switch (cwmsg->capwap_mode){
		case CWMODE_CISCO:
			/* encryption capabilities */
			*((uint16_t*)(d+len))=htons(wtpinfo->encryption_cap);
			len+=2;
			break;
		default:
			/* number of encryption elemnts */
			*(d+len)=0;
			len += 1;
			/* encryption elements */	


		/*	*(d+len)=CWTH_WBID_IEEE80211;
			uint16_t val = 0;
			*((uint16_t*)(d+len+1))=htons(val);
			len+=3;
		*/


			break;
	}


	/* hardware version subelem*/
	len+=wtpdesc_addsubelem(d+len,CW_SUBELEM_WTP_HARDWARE_VERSION,
			wtpinfo->hardware_vendor_id,wtpinfo->hardware_version);

	/* software version subelem*/
	len+=wtpdesc_addsubelem(d+len,CW_SUBELEM_WTP_SOFTWARE_VERSION,
			wtpinfo->software_vendor_id,wtpinfo->software_version);

	/* bootloader version subelem*/
	len+=wtpdesc_addsubelem(d+len,CW_SUBELEM_WTP_BOOTLOADER_VERSION,
			wtpinfo->bootloader_vendor_id,wtpinfo->bootloader_version);

	cwmsg_addelem(cwmsg,CW_ELEM_WTP_DESCRIPTOR,d,len);
}
