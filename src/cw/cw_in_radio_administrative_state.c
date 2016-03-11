
#include "action.h"
#include "dbg.h"
#include "log.h"
#include "mbag.h"
#include "cw.h"
#include "radio.h"



int cw_in_radio_administrative_state(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from)
{
	int rid = cw_get_byte(data);
	int state = cw_get_byte(data+1);
	return cw_radio_set_admin_state(conn->radios,rid,state,-1);
}



