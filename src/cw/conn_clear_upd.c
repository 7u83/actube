
#include "conn.h"

#include "mavl.h"


void conn_clear_upd(struct conn *conn, int merge)
{
	if (merge){
		mavl_merge(conn->config, conn->config_upd);

		MAVLITER_DEFINE (it,conn->radios_upd);

		mavliter_foreach(&it){
			struct mbag_item * ruitem = mavliter_get(&it);
			mavl_t radio_upd = ruitem->data;

			mbag_t radio = mbag_i_get_mbag(conn->radios,ruitem->iid,NULL);
			if (radio){
				mavl_merge(radio,radio_upd);
			}

		}		
		
	}
	mavl_del_all(conn->config_upd);
	mavl_del_all(conn->radios_upd);

}

