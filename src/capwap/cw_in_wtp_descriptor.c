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
#include "capwap_items.h"

#include "wtpinfo.h"

#include "cw_util.h"
#include "log.h"




static int readelem_wtp_descriptor(struct conn *conn, struct cw_action_in *a, uint8_t *data, int len, int cq)
{

	if (len<6)
		return -1;

	cw_itemstore_t itemstore = conn->incomming;

	cw_itemstore_set_byte(itemstore,CW_ITEM_WTP_MAX_RADIOS,cw_get_byte(data));
	cw_itemstore_set_byte(itemstore,CW_ITEM_WTP_RADIOS_IN_USE,cw_get_byte(data+1));

	int ncrypt = cw_get_byte(data+2);
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
			cw_dbg(DBG_ELEM_ERR,"WTP descriptor subelement to long, length=%d>%d",i+8,len);
			return -1;
		}

		uint32_t vendor_id=cw_get_dword(data+i); //ntohl(*((uint32_t*)(msgelem+i)));

		uint32_t val = cw_get_dword(data+i+4);
		int subtype= (val>>16)&0xffff;
		int sublen = val&0xffff;
		i+=8;

		if (sublen+i>len){
			cw_dbg(DBG_ELEM_ERR,"WTP descriptor subelement too long, length = %d",sublen);
			return -1;
		}

		cw_dbg(DBG_ELEM,"Reading WTP descriptor subelement, type=%d,len=%d",subtype,sublen);
	
		switch(subtype){
			case CW_SUBELEM_WTP_HARDWARE_VERSION:
				cw_itemstore_set_dword(itemstore,CW_ITEM_WTP_HARDWARE_VENDOR,vendor_id);
				cw_itemstore_set_bstrn(itemstore,CW_ITEM_WTP_HARDWARE_VERSION,data+i,sublen);
				break;
			case CW_SUBELEM_WTP_SOFTWARE_VERSION:
				cw_itemstore_set_dword(itemstore,CW_ITEM_WTP_SOFTWARE_VENDOR,vendor_id);
				cw_itemstore_set_bstrn(itemstore,CW_ITEM_WTP_SOFTWARE_VERSION,data+i,sublen);
				break;
			case CW_SUBELEM_WTP_BOOTLOADER_VERSION:
				cw_itemstore_set_dword(itemstore,CW_ITEM_WTP_BOOTLOADER_VENDOR,vendor_id);
				cw_itemstore_set_bstrn(itemstore,CW_ITEM_WTP_BOOTLOADER_VERSION,data+i,sublen);
				break;
			default:
				cw_dbg(DBG_ELEM_ERR,"Unknown WTP descriptor subelement, type = %d",subtype);
				break;
		}
		i+=sublen;

	}while(i<len);

	return 1;
}

int cw_in_wtp_descriptor(struct conn *conn,struct cw_action_in * a,uint8_t *data,int len)
{
	int rc =readelem_wtp_descriptor(conn, a,data,len,0);
	if (rc==-1){
		cw_dbg(DBG_CW_RFC,"Bad WTP descriptor, trying cisco hack");
		rc =readelem_wtp_descriptor(conn, a,data,len,1);
	}

	return rc;
}

