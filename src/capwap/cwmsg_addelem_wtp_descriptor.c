
#include <string.h>

#include "capwap.h"


static inline int wtpdesc_addsubelem(uint8_t * dst,uint8_t type,uint32_t vendorid,uint8_t * str)  
{
//	printf("add subelem\n");
	int l;
	*((uint32_t*)(dst))=htonl(vendorid);   
//	printf("htonl done\n");
	l=strlen((char*)str);
//	printf("strlne got %d\n",l);
	*((uint32_t*)(dst+4))=htonl((type<<16)|l);
//	printf("memcopy str %d\n",l);
	memcpy(dst+8,str,l);
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

	/* number of encryption elemnts */
	*(d+len)=1;
	len+=1;

	/* encryption elements */	
	*(d+len)=CWTH_WBID_IEEE80211;
	uint16_t val = 0;
	*((uint16_t*)(d+len+1))=htons(val);
	len+=3;


	/* hardware subelem*/
	len+=wtpdesc_addsubelem(d+len,CWMSGSUBELEM_WTP_DESCRIPTOR_HARDWARE_VERSION,
			wtpinfo->hardware_vendor_id,wtpinfo->hardware_version);


	/* software subelem*/
	len+=wtpdesc_addsubelem(d+len,CWMSGSUBELEM_WTP_DESCRIPTOR_SOFTWARE_VERSION,
			wtpinfo->software_vendor_id,wtpinfo->software_version);

	/* bootloader subelem*/
	len+=wtpdesc_addsubelem(d+len,CWMSGSUBELEM_WTP_DESCRIPTOR_BOOTLOADER_VERSION,
			wtpinfo->bootloader_vendor_id,wtpinfo->bootloader_version);


	cwmsg_addelem(cwmsg,CWMSGELEM_WTP_DESCRIPTOR,d,len);
}
