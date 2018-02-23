/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
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
 * @brief Implements cw_in_capwap_control_ip_address
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#include <errno.h>

#include "action.h"
#include "log.h"
#include "mbag.h"
#include "cw.h"
#include "capwap_items.h"
#include "aciplist.h"
#include "sock.h"



int cw_in_capwap_control_ip_address(struct conn *conn, struct cw_action_in *a,
				 uint8_t * data, int len,struct sockaddr *from)
{
	cw_aciplist_t list =
	    mbag_get_mavl_c(conn->incomming,a->item_id,cw_aciplist_create);

	if (!list) {
		cw_log(LOG_ERR, "Error: Can't allocate CAWPAP IP Adress List");
		return 0;
	}

	cw_acip_t * acip;
	acip = malloc(sizeof(cw_acip_t));
	if (!acip) {
		cw_log(LOG_ERR,"Can't allocate memory for acv4ip: %s",strerror(errno));
			return 0;
	}

	
	if (a->elem_id == CW_ELEM_CAPWAP_CONTROL_IPV4_ADDRESS) {
		struct sockaddr_in addr;
		memcpy(&addr.sin_addr,data,4);
		addr.sin_family=AF_INET;
		sock_setport((struct sockaddr*)&addr,CAPWAP_CONTROL_PORT);
		memcpy(&acip->ip,&addr,sizeof(addr));
		acip->index = cw_get_word(data+4);
	}

	if (a->elem_id == CW_ELEM_CAPWAP_CONTROL_IPV6_ADDRESS) {
		struct sockaddr_in6 addr;
		memset (&addr,0,sizeof(addr));
		memcpy(&addr.sin6_addr,data,16);
		addr.sin6_family=AF_INET6;
		sock_setport((struct sockaddr*)&addr,CAPWAP_CONTROL_PORT);
		memcpy(&acip->ip,&addr,sizeof(addr));
		acip->index = cw_get_word(data+16);
	}


	cw_aciplist_replace(list,acip);

	return 1;
}

