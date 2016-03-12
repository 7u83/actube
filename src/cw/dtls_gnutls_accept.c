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
#include "dtls_gnutls.h"
#include "timer.h"


int dtls_gnutls_accept(struct conn *conn)
{
	struct dtls_gnutls_data *d;

	gnutls_datum_t cookie_key;

	gnutls_key_generate(&cookie_key, GNUTLS_COOKIE_KEY_SIZE);
	cw_dbg(DBG_DTLS, "Session cookie for %s generated: %s",
	       sock_addr2str(&conn->addr), sock_hwaddr2idstr((uint8_t *) (&cookie_key),
							     sizeof(cookie_key)));

	gnutls_dtls_prestate_st prestate;
	memset(&prestate, 0, sizeof(prestate));



	uint8_t buffer[2048];
	int tlen;
	tlen = dtls_gnutls_bio_read(conn, buffer, sizeof(buffer));

	gnutls_dtls_cookie_send(&cookie_key, &conn->addr, sizeof(conn->addr),
				&prestate, (gnutls_transport_ptr_t) conn, dtls_gnutls_bio_write);

	int rc=-1;

	time_t c_timer = cw_timer_start(10);

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

	//	dtls_gnutls_bio_read(conn, buffer, sizeof(buffer));
		break;

	}

	if (rc <0 ){
		cw_log(LOG_ERR, "Cookie couldn't be verified: %s", gnutls_strerror(rc));
		return 0;
	}


	cw_dbg(DBG_DTLS, "Cookie verified! Starting handshake with %s ...",sock_addr2str(&conn->addr));



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
		cw_log(LOG_ERR, "Error in handshake with %s: %s",sock_addr2str(&conn->addr), gnutls_strerror(rc));
		return 0;
	}


	cw_dbg(DBG_DTLS,"Handshake with %s successful.",sock_addr2str(&conn->addr));

	conn->dtls_data=d;
	conn->read = dtls_gnutls_read;
	conn->write = dtls_gnutls_write;

	return 1;
}


