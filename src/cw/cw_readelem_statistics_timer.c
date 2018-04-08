#include "capwap.h"

#include "dbg.h"

int cw_readelem_statistics_timer(uint16_t *timer, int type, uint8_t * msgelem, int len)
{
	if (type != CAPWAP_ELEM_STATISTICS_TIMER)
		return 0;

	if (len!=2){
		cw_dbg(DBG_RFC,"Statistics timer msgelem has wrong size, type=%d,len=%d",type,len);
		return 0;
	}
	*timer=*((uint16_t*)msgelem);
	return 1;
}


