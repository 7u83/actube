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
 * @brief Implementation output handler of capwap cw_out_local_ip_address
 */

#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include "cw.h"
#include "capwap_items.h"

#include "dbg.h"
#include "log.h"
#include "conn.h"


/**
 * Output handler for Capwap Local IP Address message element.
 *
 * @param conn Connection object
 * @param action Pointer to action which called this handler
 * @param dst Destination buffer
 *
 * This handler determines the IP address from #conn->sock.
 * It can deal both with IPv4 and IPv6 sockets.
 *
 */
int cw_out_wtp_ip_address(struct conn *conn, struct cw_action_out *action,
				   uint8_t * dst)
{
	return cw_put_local_ip_address(conn->sock,dst,CW_ELEM_WTP_IPV4_IP_ADDRESS,CW_ELEM_WTP_IPV6_IP_ADDRESS);
}
