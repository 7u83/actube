

#include "cw/action.h"


int capwap_in_session_id(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len, struct sockaddr *from){

	memcpy(conn->session_id,data,len);
	return 1;
}

