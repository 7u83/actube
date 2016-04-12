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
 * @brief conn_init
 */ 

#include <string.h>

#include "conn.h"
#include "capwap.h"

/**
 * Basic initialization of a conn object 
 * @param conn conn object to initialize
 */ 
void conn_init(struct conn * conn)
{
	memset(conn,0,sizeof(struct conn));
	conn->retransmit_interval=CAPWAP_RETRANSMIT_INTERVAL;
	conn->max_retransmit=CAPWAP_MAX_RETRANSMIT;
	conn->wait_dtls=CAPWAP_WAIT_DTLS;
	conn->wait_join=CAPWAP_WAIT_JOIN;
	conn->mtu_discovery=1;
	conn->capwap_mode = CW_MODE_AUTO;
	conn->strict_capwap=1;

	conn->remote = mbag_create();

	conn->process_packet=conn_process_packet;
	conn->process_message=process_message;


	conn->config = mbag_create();
	conn->config_upd=mbag_create();
}


