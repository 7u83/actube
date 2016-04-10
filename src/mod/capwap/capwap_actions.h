#ifndef __CAPWAP_ACTIONS_H
#define __CAPWAP_ACTIONS_H

int capwap_in_session_id(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len, struct sockaddr *from);

#endif
