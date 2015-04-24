
#include "action.h"
#include "dbg.h"
#include "log.h"
#include "mbag.h"
#include "capwap.h"



int cw_in_radio_administrative_state(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from)
{
	int rid = cw_get_byte(data);
	
	mbag_get_mbag(conn->radios,rid,NULL);
	

	return 1;
}
