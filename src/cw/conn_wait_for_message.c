
#include "conn.h"
#include "cw_util.h"


struct cwrmsg * conn_wait_for_message(struct conn * conn, time_t timer)
{
	struct cwrmsg * cwrmsg;


	while (!cw_timer_timeout(timer)){
		cwrmsg = conn_get_message(conn);

		if (!cwrmsg){
			if (!conn_is_error(conn))
				continue;

			return 0;
		}

		if (cwrmsg->type & 1){
			if (conn->request_handler){
				if (conn->request_handler(conn->request_handler_param))
					continue;
			}

			
		}

		return cwrmsg;

	}

	return 0;
}


