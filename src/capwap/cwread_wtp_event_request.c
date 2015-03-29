#include <stdio.h>

#include "capwap.h"
#include "cw_log.h"
#include "cw_util.h"

struct eparm {
	int * mand;
	uint32_t result_code;
	struct wtpinfo * wtpinfo;
};


static int readelem(void * eparm,int type,uint8_t* msgelem,int len)
{
	struct eparm * e = (struct eparm*)eparm;
	cw_dbg_msgelem(CW_MSG_CHANGE_STATE_EVENT_REQUEST,type,msgelem,len);



	return 0;
//foundX:
	cw_mand_elem_found(e->mand,type);
	return 1;
}


int cwread_wtp_event_request(struct wtpinfo * wtpinfo, uint8_t * msg, int len)
{
	int mand[] = {
		-1};

	struct eparm eparm;
	eparm.wtpinfo = wtpinfo;
	eparm.mand=mand;

	cw_dbg(DBG_ELEM,"Reading wtp event request, len=%d",len);
	cw_foreach_msgelem(msg,len,readelem,&eparm);


	if (cw_is_missing_mand_elems(mand)){
		char str[512];
		cw_get_missing_mand_elems(str,mand);
		cw_dbg(DBG_CW_RFC, "Missing msgelems in change state event request: %s",str);
	}
	return eparm.result_code;
}





