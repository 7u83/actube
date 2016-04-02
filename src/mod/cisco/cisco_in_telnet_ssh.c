

#include "cisco.h"
#include "include/cipwap_items.h"
#include "cw/cw.h"
#include "cw/dbg.h"

int cisco_in_telnet_ssh(struct conn *conn, struct cw_action_in *a, uint8_t * data, int len,
		  struct sockaddr *from)
{
	int enable=lw_get_byte(data);
	int type = lw_get_byte(data+1);


	switch (type){
		case 0:
			mbag_set_byte(conn->incomming,CIPWAP_ITEM_TELNET_ENABLE,enable);
			break;
		case 1:
			mbag_set_byte(conn->incomming,CIPWAP_ITEM_SSH_ENABLE,enable);
			break;
		default:
			cw_dbg(DBG_ELEM_ERR,"Unknown Telnet/SSH Type: %d",type);
			break;
	}

	return 0;

}
