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

#include <stdio.h>
#include <string.h>

#include "capwap.h"
#include "lwapp.h"

#include "wtpinfo.h"

#include "cw_util.h"

int wtpinfo_lwreadelem_wtp_descriptor(struct wtpinfo * wtpinfo, int type, uint8_t *msgelem, int len)
{
	if (type != LW_ELEM_WTP_DESCRIPTOR)
		return 0;

/*	if (len!=16)
		return -1;
*/

	char  str[64];	
	uint32_t hwversion=ntohl(*((uint32_t*)(msgelem)));
	sprintf(str,"%08X",hwversion);
	cw_setstr(&wtpinfo->hardware_version,(uint8_t*)str,strlen(str));

	uint32_t swversion=ntohl(*((uint32_t*)(msgelem+4)));
	sprintf(str,"%08X",swversion);
	cw_setstr(&wtpinfo->software_version,(uint8_t*)str,strlen(str));

	uint32_t btversion=ntohl(*((uint32_t*)(msgelem+8)));
	sprintf(str,"%08X",btversion);
	cw_setstr(&wtpinfo->bootloader_version,(uint8_t*)str,strlen(str));

	uint32_t l=ntohl(*((uint32_t*)(msgelem+12)));
	
	wtpinfo->max_radios = l >> 24;
	wtpinfo->radios_in_use = (l>>16) & 0xff;


	return 1;
}

