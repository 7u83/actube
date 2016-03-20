#include "cw/radio.h"
#include "cw/action.h"
#include "cw/cw.h"



int cisco_in_radio_administrative_state_wtp(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from)
{

	int rid = cw_get_byte(data);
	int state = cw_get_byte(data+1);

	printf("Set admin state for %d = %d\n",rid,state);


	if (rid != 255)
		return cw_radio_set_admin_state(conn->radios,rid,state,3);

	

	return cw_radio_set_all_admin_states(conn->radios,state,3);

}


