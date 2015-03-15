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

#include <string.h>
#include "capwap.h"

void cwmsg_addelem_ac_descriptor(struct cwmsg *msg,struct ac_info * acinfo)
{
	uint8_t buffer[12+2048];
	uint8_t * acd = buffer;

	*((uint32_t*)(acd))=htonl((acinfo->stations<<16) | (acinfo->limit));
	*((uint32_t*)(acd+4))=htonl((acinfo->active_wtps<<16) | acinfo->max_wtps);
	*((uint32_t*)(acd+8))=htonl((acinfo->security<<24) | (acinfo->rmac<<16) | acinfo->dtls_policy );


	int len = 12;
	int sublen;

	sublen = 4;
		
	*((uint32_t*)(acd+len))=htonl(CW_VENDOR_ID_CISCO);
	len+=4;
	*((uint32_t*)(acd+len))=htonl((1<<16)|sublen);
	len+=4;
	*(acd+len)=5; len++;
	*(acd+len)=0; len++;
	*(acd+len)=72; len++;
	*(acd+len)=71; len++;
	*(acd+len)=5; len++;
	*(acd+len)=6; len++;
	*(acd+len)=7; len++;
	*(acd+len)=8; len++;


	*((uint32_t*)(acd+len))=htonl(CW_VENDOR_ID_CISCO);
	len+=4;
	*((uint32_t*)(acd+len))=htonl((1<<16)|sublen);
	len+=4;
	*(acd+len)=7; len++;
	*(acd+len)=3; len++;
	*(acd+len)=1; len++;
	*(acd+len)=72; len++;
	*(acd+len)=5; len++;
	*(acd+len)=6; len++;
	*(acd+len)=7; len++;
	*(acd+len)=8; len++;




goto b;

	/* software version subelement */

	*((uint32_t*)(acd+len))=htonl(CW_VENDOR_ID_CISCO);
	len+=4;
	sublen=strlen((const char*)acinfo->software_version);
	*((uint32_t*)(acd+len))=htonl((1<<16)|sublen);
	len+=4;
	memcpy(acd+len,acinfo->software_version,sublen);
	len+=sublen;


	/* hardware version subelement */
	*((uint32_t*)(acd+len))=htonl(CW_VENDOR_ID_CISCO); 
	len+=4;
	sublen=strlen((const char*)acinfo->hardware_version);
	*((uint32_t*)(acd+len))=htonl((4<<16)|sublen);
	len+=4;
	memcpy(acd+len,acinfo->hardware_version,sublen);
	len+=sublen;
b:
	cwmsg_addelem(msg,CWMSGELEM_AC_DESCRIPTOR,acd,len);

}


