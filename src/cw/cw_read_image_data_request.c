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

/**
 *@file
 *@brief Image Data handling
 */
#include <string.h>

#include "capwap.h"

#include "cw_log.h"
#include "cw_util.h"

#include <stdio.h> 

int cw_readelem_image_identifier(struct cwimage_data *data, int type,uint8_t *msgelem, int len)
{
	if (type != CW_ELEM_IMAGE_IDENTIFIER)
		return 0;

	data->vendor_id = ntohl(*((uint32_t*)msgelem));

	if (len >= 1024) {
		cw_dbg(DBG_MSG_ERR,"Image identifier too long (>1024), truncating");
		len = 1024;
	}
	
	if ( data->identifier ){
		memcpy(data->identifier,msgelem,len);
		*(data->identifier+len)=0;
	}

	return 1;
}

static int imgdata_request(void * ptr,int type,uint8_t* msgelem,int len)
{
	cw_dbg_msgelem(CW_MSG_IMAGE_DATA_REQUEST, type, msgelem, len);


	cw_dbg(DBG_ALL,"Reading image data req msgelem, type=%d - %s ,len=%d\n",type,cw_strelem(type),len);

	if (cw_readelem_image_identifier(ptr,type,msgelem,len))
		return 1;

	return 0;
}


/**
 * Read an image data request message
 */
void cw_read_image_data_request(struct cwimage_data *data, uint8_t * msg, int len)
{
	cw_foreach_msgelem(msg,len,imgdata_request,data);
}



