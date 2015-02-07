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




static int wtpinfo_readelem_wtp_descriptor_(struct wtpinfo * wtpinfo, int type, uint8_t *msgelem, int len, int cq)
{
	if (type != CWMSGELEM_WTP_DESCRIPTOR)
		return 0;

	if (len<6)
		return -1;

	wtpinfo->max_radios=*msgelem;
	wtpinfo->radios_in_use=*(msgelem+1);

	int ncrypt = *(msgelem+2);
	int i;
	if (ncrypt == 0 ){
		/* non-conform */
		cw_dbg(DBG_CW_RFC,"Non-standard-conform WTP descriptor detected (See RFC 5415)");
		if (!cq) 
			i=3;
		else
			i=4;
	}
	else{
		i=ncrypt*3+3;
	}

	do {
		if (i+8>len)
		{
			cw_dbg(DBG_CW_MSG_ERR,"WTP descriptor subelement to long, length=%d>%d",i+8,len);
			return -1;
		}

		uint32_t vendor_id=ntohl(*((uint32_t*)(msgelem+i)));

		uint32_t val = ntohl(*((uint32_t*)(msgelem+i+4)));
		int subtype= (val>>16)&0xffff;
		int sublen = val&0xffff;
		i+=8;

		if (sublen+i>len){
			cw_dbg(DBG_CW_MSG_ERR,"WTP descriptor subelement too long, length = %d",sublen);
			return -1;
		}

		cw_dbg(DBG_CW_MSG,"Reading WTP descriptor subelement, type=%d,len=%d",subtype,sublen);
	
		switch(subtype){
			case CWMSGSUBELEM_WTP_DESCRIPTOR_HARDWARE_VERSION:
				wtpinfo->hardware_vendor_id=vendor_id;
				cw_setstr(&wtpinfo->hardware_version,msgelem+i,sublen);
				wtpinfo->hardware_version_len=sublen;
				break;
			case CWMSGSUBELEM_WTP_DESCRIPTOR_SOFTWARE_VERSION:
				wtpinfo->software_vendor_id=vendor_id;
				cw_setstr(&wtpinfo->software_version,msgelem+i,sublen);
				wtpinfo->software_version_len=sublen;
				break;
			case CWMSGSUBELEM_WTP_DESCRIPTOR_BOOTLOADER_VERSION:
				wtpinfo->bootloader_vendor_id=vendor_id;
				cw_setstr(&wtpinfo->bootloader_version,msgelem+i,sublen);
				wtpinfo->bootloader_version_len=sublen;
				break;
			default:
				cw_dbg(DBG_CW_MSG_ERR,"Unknown WTP descriptor subelement, type = %d",subtype);
				break;
		}
		i+=sublen;

	}while(i<len);

	return 1;
}

int wtpinfo_readelem_wtp_descriptor(struct wtpinfo * wtpinfo, int type, uint8_t *msgelem, int len)
{
	int rc =wtpinfo_readelem_wtp_descriptor_(wtpinfo, type, msgelem, len,0);
	if (rc==-1){
		cw_dbg(DBG_CW_RFC,"Bad wtp descriptor, trying cisco hack");
		rc =wtpinfo_readelem_wtp_descriptor_(wtpinfo, type, msgelem, len,1);
	}

	return rc;
}

