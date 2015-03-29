
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
 * @file 
 * @brief Implements cw_send_image_data_response
 */

#include "sock.h"
#include "capwap.h"
#include "cw_log.h"


void cw_send_image_data_response(struct conn * conn,int seqnum, int rc)
{
	cw_dbg(DBG_MSG,"Sending image data response to %s, seq = %d",sock_addr2str(&conn->addr),seqnum);

	struct cwmsg * cwmsg = &conn->resp_msg;	
	cwmsg_init(cwmsg,conn->resp_buffer,CW_MSG_IMAGE_DATA_RESPONSE,seqnum,NULL);

	cwmsg_addelem_result_code(cwmsg,rc);
	conn_send_response(conn,cwmsg,seqnum);

}
