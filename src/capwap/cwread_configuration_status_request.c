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
 * @file
 * @brief Implents configuration status request 
 */ 

#include <stdio.h>

#include "capwap.h"
#include "cw_log.h"
#include "cw_util.h"

struct eparm{
	int * mand;
	struct wtpinfo * wtpinfo;	

};

static int readelem(void * eparm,int type,uint8_t* msgelem,int len)
{
	struct eparm * e = (struct eparm*)eparm;
	cw_dbg_msgelem(CWMSG_CONFIGURATION_STATUS_REQUEST, type, msgelem,len);

	/* mandatory elements */
	
	/* ac name */
	if (cw_readelem_ac_name(&e->wtpinfo->ac_name,type,msgelem,len))
		goto foundX;

	/* radio administrative state */
	if (cw_readelem_radio_administrative_state(e->wtpinfo->radioinfo, type,msgelem,len))
		goto foundX;
	
	/* statistics timer */
	if (cw_readelem_statistics_timer(&e->wtpinfo->statistics_timer, type,msgelem,len))
		goto foundX;

	/* reboot statistics */
	if (cw_readelem_wtp_reboot_statistics(&e->wtpinfo->reboot_statistics,type,msgelem,len))
		goto foundX;

	/* non-mantatory elements */

	if (cw_readelem_vendor_specific_payload
	    (e->wtpinfo, CWMSG_CONFIGURATION_STATUS_REQUEST, type, msgelem, len))
		return 1;


	return 0;
foundX:
	cw_mand_elem_found(e->mand,type);
	return 1;

}

void cwread_configuration_status_request(struct wtpinfo * wtpinfo, uint8_t * msg, int len)
{
	int mand[] = {
		CWMSGELEM_AC_NAME,
		CWMSGELEM_WTP_REBOOT_STATISTICS,
		CWMSGELEM_RADIO_ADMINISTRATIVE_STATE,
		CWMSGELEM_STATISTICS_TIMER,
		-1};

	struct eparm eparm;
	eparm.wtpinfo = wtpinfo;
	eparm.mand=mand;

	cw_dbg(DBG_CW_MSGELEM,"Reading configuration status request, len=%d",len);
	cw_foreach_msgelem(msg,len,readelem,&eparm);

	if (cw_is_missing_mand_elems(mand)){
		char str[512];
		cw_get_missing_mand_elems(str,mand);
		cw_dbg(DBG_CW_RFC, "Missing msgelems in configuration status request: %s",str);
	}
}

