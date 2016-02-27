#ifndef __MOD_CISCO_H
#define __MOD_CISCO_H

struct mod_ac * mod_cisco_ac();

extern int cisco_in_wtp_descriptor(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len, struct sockaddr *from);


#endif
