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
 * @brief Implement conn_create_noq
 */ 

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "conn.h"
#include "sock.h"

#include "log.h"

/**
 * Create a connection object without queueing
 * @retval 1 Success
 * @retval 0 failure, conslt errno for more details
 */
struct conn * conn_create_noq(int sock, struct sockaddr * addr)
{
	struct conn * conn; 
	conn = malloc(sizeof (struct conn));
	if (!conn)
		return NULL;

	conn_init(conn);


	conn->sock=sock;

	if (addr)
		sock_copyaddr(&conn->addr,addr);


	/* create the CAPWAP framentation manager */
	conn->fragman = fragman_create();
	if (conn->fragman==NULL){
		conn_destroy(conn);
		return NULL;
	}

	/* set packet receive and send methods */
	conn->recv_packet = conn_recv_packet;
	conn->recv_packet_peek = conn_recv_packet_peek;
	conn->send_packet = conn_send_packet;


	/* misc settings */
	conn->last_seqnum_received=-1;
	conn->mtu=1500;


	conn->cur_packet=0;
	conn->recv_timeout=1;

	conn->seqnum=-1;
	conn->write = conn->send_packet;
	conn->read = conn->recv_packet;

	return conn;
}


