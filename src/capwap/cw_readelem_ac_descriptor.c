#include <stdio.h> 

#include "capwap.h"
#include "acinfo.h"
#include "bstr.h"



static int read_subelem_cisco(struct ac_info* acinfo,int subtype,uint8_t * elem, int len)
{
	switch (subtype) {
		case 0:
			/* software version */
			bstr_replace(&acinfo->software_version,bstr_create(elem,len));
			break;
		case 1: 
			/* hardware version */
			bstr_replace(&acinfo->hardware_version,bstr_create(elem,len));
			break;

		default:
			printf("What? %d\n",subtype);

	}
	
	return 1;
}


static int read_subelem(struct ac_info* acinfo,int subtype,uint8_t *elem, int len)
{
	switch (subtype){
		case 4:
			/* hardware version */
			bstr_replace(&acinfo->hardware_version,bstr_create(elem,len));
			break;
		case 5:
			/* software version */
			bstr_replace(&acinfo->software_version,bstr_create(elem,len));
			break;
	}

	return 1;
}



int cw_readelem_ac_descriptor(struct ac_info * acinfo,int type, uint8_t *msgelem, int len)
{
	if (type != CWMSGELEM_AC_DESCRIPTOR)
		return 0;

	if (len<12)
		return -1;

	uint32_t val;

	/* read stations and limit */
	val = ntohl(*((uint32_t*)msgelem));
	acinfo->stations  = val>>16;
	acinfo->limit = val&0xffff;


	/* read active wtps and max wtps */
	val = ntohl(*((uint32_t*)(msgelem+4)));
	acinfo->active_wtps  = val>>16;
	acinfo->max_wtps = val&0xffff;

	/* read active wtps and max wtps */
	val = ntohl(*((uint32_t*)(msgelem+8)));
	acinfo->security = val >>24;
	acinfo->rmac= (val >> 16)&0xff;
	acinfo->dtls_policy=val&0xf;

	int sub=12;
	int sublen;


	while (sub<len){
		if (len-sub<8)
			return -1;

		uint32_t vendor = ntohl(*((uint32_t*)(msgelem+sub)));
		val = ntohl(*((uint32_t*)(msgelem+sub+4)));
		sublen = val&0xffff;
		sub+=8;
		int subtype = val>>16;


		switch (vendor) {
			case 0:
				read_subelem(acinfo,subtype,msgelem+sub,sublen);
				break;
				
			case CW_VENDOR_ID_CISCO:
				read_subelem_cisco(acinfo,subtype,msgelem+sub,sublen);
				break;
			
		}

		if (sub+sublen>len)
			return -1;

		sub+=sublen;
	}

	return 1;	
}


