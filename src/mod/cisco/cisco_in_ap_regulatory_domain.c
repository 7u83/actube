/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
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
 * @file
 * @brief Implements cw_in_capwap_control_ip_address
 */

#include "mod_capwap.h"


int cisco_in_ap_regulatory_domain(struct cw_ElemHandler *eh, 
		struct cw_ElemHandlerParams *params, 
			uint8_t * data,	 int len)
{
	char key[CW_KTV_MAX_KEY_LEN];
	int idx;
	
	sprintf(key,"%s/address",eh->key);
	idx = cw_ktv_idx_get(params->conn->remote_cfg,key);
	
/*	printf("SKEY is %s , idx: %d\n",key,idx);*/

	sprintf(key,"%s/address.%d",eh->key,idx+1);
	cw_ktv_add(params->conn->remote_cfg,key,CW_TYPE_IPADDRESS,data,len-2);
	
	sprintf(key,"%s/wtps.%d",eh->key,idx+1);
	cw_ktv_add(params->conn->remote_cfg,key,CW_TYPE_WORD,data+len-2,2);

	return 1;
}

