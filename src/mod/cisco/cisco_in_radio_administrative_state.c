#include "cw/radio.h"
#include "cw/action.h"
#include "cw/cw.h"

#include "cisco_items.h"

int cisco_in_radio_administrative_state(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from)
{

	int rid = cw_get_byte(data);
	if (rid != 255)
		return cw_in_radio_generic(conn,a,data,len,from);


	int state = cw_get_byte(data+1);
	mbag_set_byte(conn->incomming,CISCO_ITEM_WTP_ADMIN_STATE,state);
	return 1;

}


