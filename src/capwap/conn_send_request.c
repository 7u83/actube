/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

/**
 *@file
 *@brief conn_send_request 
 */

#include "capwap.h"
#include "cw_util.h"
#include "cw_log.h"

/**
 * Send a request message and wait for its response
 * @param conn the connection
 *
 * The message has to be prepared and put to conn->req_msg
 * 
 */ 
struct cwrmsg * conn_send_request(struct conn * conn)
{
	int i;

	struct cwrmsg * cwrmsg;
	struct cwmsg * cwmsg = &conn->req_msg;


        for (i=0; i<conn->max_retransmit; i++) {

                time_t r_timer = cw_timer_start(conn->retransmit_interval);
		if (i!=0)
	                cw_dbg(DBG_MSG_ERR,"Retransmitting message, type=%d,seq=%d",cwmsg->type,cwmsg->seqnum);

		conn_send_cwmsg(conn,&conn->req_msg);
		cwrmsg = conn_wait_for_message(conn,r_timer);
		if (cwrmsg){
			if (cwrmsg->type == conn->req_msg.type+1){
				return cwrmsg;        
			}
                                
		}
        }
        cw_dbg(DBG_MSG_ERR,"Max retransmit's reached, message type=%d,seq=%d",cwmsg->type,cwmsg->seqnum);
        return 0;
}

