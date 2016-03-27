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
 * @brief 
 */ 

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "log.h"
#include "conn.h"
#include "sock.h"
#include "capwap.h"


/**
 * Create a conn object
 * @param sock a socket
 * @param addr the address associated
 * @param qsize size of packet queue
 * @return A pointer to the created object
 * This function creates a conn obnject with queueing functionality
 * for asynchronous operation. 
 * To create a conn object without queue functionallity use  #conn_create_noq.
 */
struct conn * conn_create(int sock, struct sockaddr * addr, int qsize)
{
	struct conn * conn; 
	conn = malloc(sizeof (struct conn));
	if (!conn)
		return NULL;

	conn_init(conn);

	conn->sock=sock;

	if (addr)
		sock_copyaddr(&conn->addr,addr);


	conn->fragman = fragman_create();
	if (conn->fragman==NULL){
		conn_destroy(conn);
		return NULL;
	}

	conn->qsize=qsize;
	if (qsize != 0){
		if (!(conn->q=malloc( sizeof(uint8_t *) * qsize))){
			conn_destroy(conn);
			return NULL;
		}
		conn->qrpos=-1;
		if (sem_init(&conn->q_sem,0,0)!=0){
			cw_log(LOG_ERR,"Fatal- Can't init semaphore for conn object: %s",strerror(errno));
			conn_destroy(conn);
			return NULL;
		};
		conn->recv_packet=conn_q_recv_packet;
		conn->recv_packet_peek=conn_q_recv_packet_peek;
	}
	else{
		conn->recv_packet = conn_recv_packet;
		conn->recv_packet_peek = conn_recv_packet_peek;
	}

	conn->send_packet = conn_send_packet;
	conn->send_data_packet = conn_send_data_packet;

	conn->last_seqnum_received=-1;
	conn->mtu=1500;


	conn->cur_packet=0;
	conn->recv_timeout=1;

	conn->seqnum=-1;
	conn->write = conn->send_packet;
	conn->read = conn->recv_packet;

	conn->write_data = conn->send_data_packet;

	conn->dtls_mtu = 1500;

	return conn;
}


