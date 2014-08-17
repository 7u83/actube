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

#include "cw_log.h"
#include "cw_util.h"

#include "capwap.h"


struct eparm{
	int * mand;
	struct wtpinfo * wtpinfo;	

};

static void mand_elem_found(int *l,int type)
{
	if (!cw_dbg_is_level(DBG_CW_RFC))
		return;

	int i;
	for (i=0; l[i]!=-1; i++){
		if(l[i]==type){
			l[i]=0;
			return;
		}
	
	}
}

void get_missing_mand_elems(char *dst, int *l)
{
	if (!cw_dbg_is_level(DBG_CW_RFC))
		return;

	char *s = dst;
	int i;
	const char * k = "";
	for (i=0; l[i]!=-1; i++){
		if(l[i]){
			s += sprintf(s,"%s[%s]",k,cw_msgelemtostr(l[i]));
			k=",";
		}
	
	}
}

int is_missing_mand_elems(int *l)
{
	int i;
	for (i=0; l[i]!=-1; i++){
		if(l[i]){
			return 1;
		}
	}
	return 0;
}



static int readelem(void * eparm,int type,uint8_t* msgelem,int len)
{

	struct eparm * e = (struct eparm*)eparm;
	cw_dbg(DBG_CW_MSGELEM,"Reading conf staus req msgelem, type=%d - %s ,len=%d\n",type,cw_msgelemtostr(type),len);

	/* mandatory elements */
	if (cw_readelem_ac_name(&e->wtpinfo->ac_name,type,msgelem,len))
		goto foundX;
	if (cw_readelem_wtp_reboot_statistics(&e->wtpinfo->reboot_statistics,type,msgelem,len))
		goto foundX;
	if (cw_readelem_radio_administrative_state(e->wtpinfo->radioinfo, type,msgelem,len))
		goto foundX;
	if (cw_readelem_statistics_timer(&e->wtpinfo->statistics_timer, type,msgelem,len))
		goto foundX;


	return 0;
foundX:
	mand_elem_found(e->mand,type);
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

	if (is_missing_mand_elems(mand)){
		char str[512];
		get_missing_mand_elems(str,mand);
		cw_dbg(DBG_CW_RFC, "Missing msgelems in configuration status request: %s",str);
	}
}

