

#include "capwap.h"
#include "cwmsg.h"
#include "radioinfo.h"

void cwmsg_addelem_radio_operational_state(struct cwmsg * cwmsg, struct radioinfo * ri)
{
	uint8_t s[3];
	s[0]=ri->rid;
	s[1]=ri->state;
	s[2]=ri->cause;

	cwmsg_addelem(cwmsg,CW_ELEM_RADIO_OPERATIONAL_STATE,s,3);
}
