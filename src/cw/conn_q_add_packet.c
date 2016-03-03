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

#include <stdlib.h>
#include <string.h>

#include "dbg.h"
#include "conn.h"
#include "sock.h"


void conn_q_add_packet(struct conn * conn,uint8_t *packet,int len)
{
	int qwpos = conn->qwpos;
	if (qwpos==conn->qsize)
		qwpos=0;

	if (conn->qrpos==qwpos){
		/* no buffers, discard packet */
		cw_dbg(DBG_PKT_ERR, "Discarding packet from %s, no queue buffers left",
			sock_addr2str(&conn->addr));
		return;
	}

	conn->q[qwpos]=malloc(len+4);
	if (conn->q[qwpos]==NULL)
		return;
	
	*((uint32_t*)(conn->q[qwpos]))=len;
	memcpy(conn->q[qwpos]+4,packet,len);	
	conn->qwpos=qwpos+1;

	sem_post(&conn->q_sem);
}



