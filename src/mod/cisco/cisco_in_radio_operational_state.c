
#include "action.h"
#include "dbg.h"
#include "log.h"
#include "mbag.h"
#include "capwap.h"



int cisco_in_radio_operational_state(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			int len, struct sockaddr *from)
{
	int rid = cw_get_byte(data);
	int state = cw_get_byte(data+1); 
	int cause = cw_	get_byte(data+3);

	if (state 


	return 1;
}

