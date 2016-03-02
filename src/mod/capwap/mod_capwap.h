#ifndef __MOD_CAPWAP_H
#define __MOD_CAPWAP_H

struct mod_ac * mod_capwap_ac();
struct mod_wtp * mod_capwap_wtp();


extern int capwap_in_wtp_descriptor(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len, struct sockaddr *from);

int capwap_in_wtp_board_data(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len, struct sockaddr *from);


#endif
