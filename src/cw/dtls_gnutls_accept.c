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
#include <errno.h>

#include <gnutls/gnutls.h>
#include <gnutls/dtls.h>



#include "conn.h"
#include "dbg.h"
#include "log.h"
#include "sock.h"
#include "capwap.h"
#include "dtls_common.h"
#include "dtls_gnutls.h"
#include "timer.h"


int dtls_gnutls_accept(struct conn *conn)
{
	char sock_buf[SOCK_ADDR_BUFSIZE];
	char cookie_buf[SOCK_ADDR_BUFSIZE];
	struct dtls_gnutls_data *d;
	uint8_t buffer[2048];
	int tlen, rc;
	time_t c_timer;

	gnutls_datum_t cookie_key;
	gnutls_dtls_prestate_st prestate;
	
	gnutls_key_generate(&cookie_key, GNUTLS_COOKIE_KEY_SIZE);
	cw_dbg(DBG_DTLS, "Session cookie for %s generated: %s",
	       sock_addr2str(&conn->addr,sock_buf), 
	       sock_hwaddrtostr((uint8_t *) (&cookie_key),
		sizeof(cookie_key),cookie_buf,""));


	memset(&prestate, 0, sizeof(prestate));

	/*tlen = dtls_gnutls_bio_read(conn, buffer, sizeof(buffer));*/

	gnutls_dtls_cookie_send(&cookie_key, &conn->addr, sizeof(conn->addr),
				&prestate, (gnutls_transport_ptr_t) conn, dtls_gnutls_bio_write);

	rc=-1;



	c_timer = cw_timer_start(10);

	while(!cw_timer_timeout(c_timer)){

		tlen = conn_q_recv_packet_peek(conn,buffer,sizeof(buffer));

		if (tlen <0 && errno == EAGAIN)
			continue;
		if (tlen < 0 ){
			/* something went wrong, iwe should log a message */
			continue;
		}
		
		rc = gnutls_dtls_cookie_verify(&cookie_key,
					       &conn->addr,
					       sizeof(conn->addr), buffer+4, tlen-4, &prestate);

		if (rc<0){
			cw_dbg(DBG_DTLS, "Cookie couldn't be verified: %s", gnutls_strerror(rc));
			dtls_gnutls_bio_read(conn, buffer, sizeof(buffer));
			continue;
		}

		break;

	}

	if (rc <0 ){
		cw_log(LOG_ERR, "Cookie couldn't be verified: %s", gnutls_strerror(rc));
		return 0;
	}


	cw_dbg(DBG_DTLS, "Cookie verified! Starting handshake with %s ...",sock_addr2str(&conn->addr,sock_buf));



	d = dtls_gnutls_data_create(conn,GNUTLS_SERVER | GNUTLS_DATAGRAM);
	if (!d)
		return 0;

	gnutls_certificate_server_set_request(d->session,GNUTLS_CERT_REQUEST);
	gnutls_dtls_prestate_set(d->session, &prestate);

	c_timer = cw_timer_start(10);
	do{
		rc = gnutls_handshake(d->session);
	}while(!cw_timer_timeout(c_timer) && rc==GNUTLS_E_AGAIN);
	


	if ( rc < 0 ) {
		cw_log(LOG_ERR, "Error in handshake with %s: %s",sock_addr2str(&conn->addr,sock_buf), gnutls_strerror(rc));
		return 0;
	}


	cw_dbg(DBG_DTLS,"Handshake with %s successful.",sock_addr2str(&conn->addr,sock_buf));

	conn->dtls_data=d;
	conn->read = dtls_gnutls_read;
	conn->write = dtls_gnutls_write;

	return 1;
}


