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


#include <string.h>

#include "conn.h"


int conn_send_response(struct conn * conn,struct cwmsg * cwmsg,int seqnum)
{
	conn->last_response = cwmsg;
//	conn->last_response_seqnum=seqnum;
//	cwmsg_send(cwmsg,seqnum,rid,conn);
	conn_send_cwmsg(conn,cwmsg); //,seqnum);
	return 1;
}


