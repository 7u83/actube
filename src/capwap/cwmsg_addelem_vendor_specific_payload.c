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


#include <stdlib.h>
#include <string.h>

#include "capwap.h"
#include "cw_log.h"

void cwmsg_addelem_vendor_specific_payload(struct cwmsg *msg,int vendor_id, int type, uint8_t * payload,int len)
{
	uint8_t * buffer = malloc (len + 6);
	if (!buffer){
		cw_log(LOG_ERR,"Add message elem vendor specific payload: out of memory");
		return;
	}

	*((uint32_t*)buffer) = htonl(vendor_id);
	*((uint16_t*)(buffer+4)) = htons(type);
	memcpy(buffer+6,payload,len);


	cwmsg_addelem(msg,CWMSGELEM_VENDOR_SPECIFIC_PAYLOAD,buffer,len+6);
	free(buffer);
}


