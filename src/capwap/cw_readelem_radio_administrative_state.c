
#include "capwap.h"
#include "cw_log.h"






int cw_readelem_radio_administrative_state(struct radioinfo * radioinfo, int type,uint8_t *msgelem, int len)
{
	if (type != CWMSGELEM_RADIO_ADMINISTRATIVE_STATE)
		return 0;
	
	if (len<2)
		return 0;

	if (len!=2){
		cw_dbg(DBG_CW_RFC,"Radio administrative state size != 2, RFC 5415");
	}

	int radioid = msgelem[0];

	radioid&=0x1f;

	radioinfo[radioid].admin_state=msgelem[1];	
	return 1; 
}
