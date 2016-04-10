#ifndef __MOD_CAPWAP_H
#define __MOD_CAPWAP_H

struct mod_ac *mod_capwap_ac();
struct mod_wtp *mod_capwap_wtp();


extern int capwap_in_wtp_descriptor(struct conn *conn, struct cw_action_in *a,
				    uint8_t * data, int len, struct sockaddr *from);

extern int capwap_in_wtp_board_data(struct conn *conn, struct cw_action_in *a,
				    uint8_t * data, int len, struct sockaddr *from);

extern int capwap_out_wtp_descriptor(struct conn *conn, struct cw_action_out *a,
				     uint8_t * dst);

extern int capwap_in_session_id(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len, struct sockaddr *from);

extern int capwap_out_ac_descriptor(struct conn *conn,struct cw_action_out * a,uint8_t *dst);

extern struct mbag_item * capwap_out_get_session_id(struct conn *conn,struct cw_action_out * a);

extern struct mbag_item * capwap_out_get_idle_timeout(struct conn *conn,struct cw_action_out * a);

extern struct mbag_item * capwap_out_get_capwap_timers(struct conn *conn,struct cw_action_out * a);

extern int capwap_out_ac_ip_list(struct conn *conn, struct cw_action_out *a, uint8_t * dst);



#endif
