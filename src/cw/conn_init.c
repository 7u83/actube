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
#include "cw.h"

static int write_header(struct cw_ElemHandler * handler, uint8_t * dst, int len)
{
	if (handler->vendor)
		return len + cw_put_elem_vendor_hdr(dst, handler->vendor, handler->id, len);

	return  len + cw_put_elem_hdr(dst, handler->id, len);
}

static int header_len(struct cw_ElemHandler * handler)
{
	return handler->vendor ? 10 : 4;
}


/**
 * Basic initialization of a conn object 
 * @param conn conn object to initialize
 */ 
void conn_init(struct cw_Conn * conn)
{
	memset(conn,0,sizeof(struct cw_Conn ));
	conn->retransmit_interval=CAPWAP_RETRANSMIT_INTERVAL;
	conn->max_retransmit=CAPWAP_MAX_RETRANSMIT;
	conn->wait_dtls=CAPWAP_WAIT_DTLS;
	conn->wait_join=CAPWAP_WAIT_JOIN;
	conn->mtu_discovery=1;
//	conn->capwap_mode = 0;
	conn->strict_capwap=1;



	conn->process_packet=conn_process_packet;
	conn->process_message=process_message;

	conn->write_header = write_header;
	conn->header_len = header_len;


}


