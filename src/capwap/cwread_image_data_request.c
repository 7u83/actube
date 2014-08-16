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
#include "acinfo.h"

#include "cw_log.h"
#include "cw_util.h"

#include <stdio.h> // tube

int cw_readelem_image_identifier(uint8_t **dst, int type,uint8_t *msgelem, int len)
{
	if (type != CWMSGELEM_IMAGE_IDENTIFIER)
		return 0;

	uint32_t vendor_id = ntohl(*((uint32_t*)msgelem));
	printf("Vendor id %d\n",vendor_id);
	
	int i;
	for(i=0; i<len; i++){
		printf("%c",msgelem[i]);
	}
	printf("\n");

	return 1;
}

static int imgdata_request(void * a,int type,uint8_t* msgelem,int len)
{

//	struct ac_info * acinfo = (struct ac_info *)a;
	cw_dbg(DBG_ALL,"Reading image data req msgelem, type=%d - %s ,len=%d\n",type,cw_msgelemtostr(type),len);

	if (cw_readelem_image_identifier(0,type,msgelem,len))
		return 1;

/*	if (acinfo_readelem_ac_descriptor(acinfo,type,msgelem,len)) 
		return 1;

	if (acinfo_readelem_ac_name(acinfo,type,msgelem,len)) 
		return 1;
	
	if (acinfo_readelem_ctrl_ip_addr(acinfo,type,msgelem,len)) 
		return 1;
*/
	return 0;
}



void cwread_image_data_request(struct ac_info * acinfo, uint8_t * msg, int len)
{
	cw_log_debug1("Reading join response");
	cw_foreach_msgelem(msg,len,imgdata_request,NULL);
}



