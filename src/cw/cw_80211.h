#ifndef __CW_80211_H
#define __CW_80211_H



int cw_in_80211_mac_operation(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len, struct sockaddr *from);

int cw_read_80211_mac_operation(uint8_t *data,mbag_t r);


#endif
