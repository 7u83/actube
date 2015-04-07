/*
    This file is part of AC-Tube.

    AC-Tube is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    AC-Tube is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "capwap/capwap.h"
#include "capwap/capwap_items.h"
#include "capwap/cw_log.h"
#include "capwap/wtpinfo.h"
#include "capwap/acinfo.h"
#include "capwap/sock.h"
#include "capwap/cw_util.h"


#include "wtp.h"
#include "wtp_conf.h"
#include "wtp_interface.h"

#include <stdio.h>



static int run_discovery(struct conn *conn)
{
	conn->capwap_state=CW_STATE_DISCOVERY;
	cw_itemstore_set_byte(conn->local,CW_ITEM_DISCOVERY_TYPE,CW_DISCOVERY_TYPE_UNKNOWN);


	cw_init_request(conn,CW_MSG_DISCOVERY_REQUEST);
	cw_put_msg(conn,conn->req_buffer);
	conn_send_msg(conn,conn->req_buffer);


	time_t timer = cw_timer_start(10);

        while (!cw_timer_timeout(timer)
               && conn->capwap_state == CW_STATE_DISCOVERY) {
                cw_read_messages(conn);
		
        }


	return 0;
}




/**
 * Run discovery for on address (eg broadcast 255.255.255.255)
 */ 
static int cw_run_discovery(struct conn *conn, const char *acaddr)
{

	/* get addr of destination */	
	struct addrinfo hints;
	struct addrinfo * res,*res0;
	memset(&hints,0,sizeof(hints));

	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = PF_UNSPEC;

	int rc = getaddrinfo(acaddr,conf_control_port,&hints,&res0);
	if(rc){
		cw_log(LOG_ERR,"Can't connect to AC %s: %s",acaddr,gai_strerror(rc));
		return 0;
	}

	for(res=res0; res; res=res->ai_next)
	{

		int sockfd;
		int opt;
		sockfd=socket(res->ai_family,SOCK_DGRAM,0);
		if (sockfd == -1){
			cw_log(LOG_ERR,"Can't create socket for %s: %s",acaddr,strerror(errno));
			continue;
		}
	
		opt = 1;
		if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt))<0){
			cw_log(LOG_ERR,"Can't set broadcast sockopt");
		}
		sock_set_recvtimeout(sockfd,1);
		sock_set_dontfrag(sockfd,0);

		sock_copyaddr(&conn->addr,res->ai_addr);
		conn->sock=sockfd;

		run_discovery(conn);

		close(sockfd);

	
	}

	freeaddrinfo(res0);

	return 0;
}


int discovery()
{
	struct conn * conn = get_conn();
	cw_run_discovery(conn,"255.255.255.255");	
	return 0;
}
