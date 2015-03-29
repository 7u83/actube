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
 * @brief Implnts send echo response
 */

#include "capwap.h"

/**
 * Send an echo response message
 * @param conn connection, see #conn
 * @param seqnum sequence number to use
 * @param radioinfo radioinfo to use, should me NULL
 * @return 1=Success\nOtherwise Error.
 */

int cw_send_echo_response(struct conn * conn,int seqnum,struct radioinfo * radioinfo)
{
	struct cwmsg * cwmsg = &conn->resp_msg;	
	cwmsg_init(cwmsg,conn->resp_buffer,CW_MSG_ECHO_RESPONSE,seqnum,radioinfo);

	conn_send_response(conn,cwmsg,seqnum);
	return 1;
}
