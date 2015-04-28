
#include "action.h"
#include "dbg.h"
#include "log.h"
#include "mbag.h"
#include "capwap.h"
#include "radio.h"




int cw_in_radio_administrative_state_wtp(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from)
{

	if (!cw_in_radio_administrative_state(conn,a,data,len,from) ) 
		return 0; 


	return 1;

}



