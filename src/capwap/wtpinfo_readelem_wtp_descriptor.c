/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/


#include "capwap.h"
#include "wtpinfo.h"

#include "cw_util.h"
#include "cw_log.h"




static int wtpinfo_readelem_wtp_descriptor_(struct wtpinfo * wtpinfo, int type, uint8_t *msgelem, int len, int capwap_mode)
{
	if (type != CW_ELEM_WTP_DESCRIPTOR)
		return 0;

	if (len<6)
		return -1;

	wtpinfo->max_radios=*msgelem;
	wtpinfo->radios_in_use=*(msgelem+1);

	int ncrypt;
	int i=2;

	switch (capwap_mode){
		case CWMODE_CISCO:
			wtpinfo->encryption_cap = ntohs( *((uint16_t*)(msgelem+2)) );
			i=4;
			break;
		default:
		{
 			ncrypt= *(msgelem+2);
			if (ncrypt == 0 ){
				/* non-conform */
				cw_dbg(DBG_CW_RFC,"Non-standard-conform WTP descriptor detected (See RFC 5415)");
				i=3;
			}
			else
				i=ncrypt*3+3;

		}
		


	}


	do {
		if (i+8>len)
		{
			cw_dbg(DBG_MSG_ERR,"WTP descriptor subelement to long, length=%d>%d",i+8,len);
			return -1;
		}

		uint32_t vendor_id=ntohl(*((uint32_t*)(msgelem+i)));

		uint32_t val = ntohl(*((uint32_t*)(msgelem+i+4)));
		int subtype= (val>>16)&0xffff;
		int sublen = val&0xffff;
		i+=8;

		if (sublen+i>len){
			cw_dbg(DBG_MSG_ERR,"WTP descriptor subelement too long, length = %d",sublen);
			return -1;
		}

		cw_dbg(DBG_MSG,"Reading WTP descriptor subelement, type=%d,len=%d",subtype,sublen);
	
		switch(subtype){
			case CW_SUBELEM_WTP_HARDWARE_VERSION:
				wtpinfo->hardware_vendor_id=vendor_id;
				cw_setstr(&wtpinfo->hardware_version,msgelem+i,sublen);
				wtpinfo->hardware_version_len=sublen;
				break;
			case CW_SUBELEM_WTP_SOFTWARE_VERSION:
				wtpinfo->software_vendor_id=vendor_id;

				bstr_replace(&wtpinfo->software_version,bstr_create(msgelem+i,sublen));
				wtpinfo->software_version_len=sublen;
				break;
			case CW_SUBELEM_WTP_BOOTLOADER_VERSION:
				wtpinfo->bootloader_vendor_id=vendor_id;
				cw_setstr(&wtpinfo->bootloader_version,msgelem+i,sublen);
				wtpinfo->bootloader_version_len=sublen;
				break;
			default:
				cw_dbg(DBG_MSG_ERR,"Unknown WTP descriptor subelement, type = %d",subtype);
				break;
		}
		i+=sublen;

	}while(i<len);

	return 1;
}

int wtpinfo_readelem_wtp_descriptor(struct wtpinfo * wtpinfo, int type, uint8_t *msgelem, int len)
{
	int rc =wtpinfo_readelem_wtp_descriptor_(wtpinfo, type, msgelem, len,CWMODE_STD);

	if (rc==-1){
		cw_dbg(DBG_CW_RFC,"Bad wtp descriptor, trying cisco");
		rc =wtpinfo_readelem_wtp_descriptor_(wtpinfo, type, msgelem, len,CWMODE_CISCO);
	}

	return rc;
}

