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
#include <errno.h>
#include <stdint.h>
#include <string.h>

#include "capwap.h"
#include "acinfo.h"
#include "cw_util.h"
#include "cw_log.h"

#include <sys/socket.h>
#include <netinet/in.h>


struct eparm {
	int *mand;
	struct ac_info *acinfo;
};



static int acinfo_readelem_discovery_resp(void * eparm,int type,uint8_t* msgelem,int len)
{
	cw_dbg_msgelem(CWMSG_DISCOVERY_RESPONSE, type, msgelem, len);
	
	struct eparm *e = (struct eparm *) eparm;


	if (acinfo_readelem_ac_descriptor(e->acinfo,type,msgelem,len))
		goto foundX;

	if (acinfo_readelem_ac_name(e->acinfo,type,msgelem,len)) 
		goto foundX;
	
	if (acinfo_readelem_ctrl_ip_addr(e->acinfo,type,msgelem,len)){
		cw_mand_elem_found(e->mand, XCWMSGELEM_CAPWAP_CONTROL_IP_ADDRESS);
		return 1;
	}

	return 0;

      foundX:
	cw_mand_elem_found(e->mand, type);
	return 1;

}



void cwread_discovery_response(struct ac_info * acinfo, uint8_t * msg, int len)
{
	int mand[] = {
		CWMSGELEM_AC_DESCRIPTOR,
		CWMSGELEM_AC_NAME,
		XCWMSGELEM_CAPWAP_CONTROL_IP_ADDRESS,
		XCWMSGELEM_CAPWAP_RADIO_INFO,

		-1
	};

	
	struct eparm eparm;
	eparm.acinfo = acinfo;
	eparm.mand = mand;


	cw_foreach_msgelem(msg,len,acinfo_readelem_discovery_resp,&eparm);

	cw_dbg_missing_mand_elems_(0, CWMSG_DISCOVERY_RESPONSE, eparm.mand);
}



