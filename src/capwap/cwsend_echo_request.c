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


#include "capwap.h"
#include "conn.h"
#include "cwmsg.h"


int cwsend_echo_request(struct conn * conn,struct radioinfo * radioinfo) //,struct wtpinfo * wtpinfo)
{
	uint8_t buffer[CWMSG_MAX_SIZE];
	struct cwmsg cwmsg;

//	cwmsg_init(&cwmsg,buffer,CWMSG_ECHO_REQUEST,conn_get_next_seqnum(conn),radioinfo);
	cwmsg_init_echo_request(&cwmsg,buffer,conn,radioinfo);
	return conn_send_cwmsg(conn,&cwmsg);
}
