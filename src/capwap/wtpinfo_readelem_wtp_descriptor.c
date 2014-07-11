#include "capwap.h"
#include "wtpinfo.h"

#include "cw_util.h"

int wtpinfo_readelem_wtp_descriptor(struct wtpinfo * wtpinfo, int type, uint8_t *msgelem, int len)
{
	if (type != CWMSGELEM_WTP_DESCRIPTOR)
		return 0;

	if (len<6)
		return -1;

	wtpinfo->max_radios=*msgelem;
	wtpinfo->radios_in_use=*(msgelem+1);

	int ncrypt = *(msgelem+2);

	int i=ncrypt*3+3;

	do {
		if (i+8>=len)
			return -1;

		uint32_t vendor_id=ntohl(*((uint32_t*)(msgelem+i)));

		uint32_t val = ntohl(*((uint32_t*)(msgelem+i+4)));
		int subtype= (val>>16)&0xffff;
		int sublen = val&0xffff;
		i+=8;

		if (sublen+i>len){
			//printf("Wrong sublen ba %d\n",sublen);
			return -1;
		}


		switch(subtype){
			case CWMSGSUBELEM_WTP_DESCRIPTOR_HARDWARE_VERSION:
				wtpinfo->hardware_vendor_id=vendor_id;
				cw_setstr(&wtpinfo->hardware_version,msgelem+i,sublen);
				break;
			case CWMSGSUBELEM_WTP_DESCRIPTOR_SOFTWARE_VERSION:
				wtpinfo->software_vendor_id=vendor_id;
				cw_setstr(&wtpinfo->software_version,msgelem+i,sublen);
				break;
			case CWMSGSUBELEM_WTP_DESCRIPTOR_BOOTLOADER_VERSION:
				wtpinfo->bootloader_vendor_id=vendor_id;
				cw_setstr(&wtpinfo->bootloader_version,msgelem+i,sublen);
				break;
			default:
				//printf("unknown !!!\n");
				break;
		}
		i+=sublen;

	}while(i<len);

	return 1;
}


