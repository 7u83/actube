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


#include "dtls_gnutls.h"

#include "conn.h"
#include "cw_log.h"
#include "sock.h"
#include "cw_util.h"


struct dtls_gnutls_data {
	gnutls_session_t session;
	gnutls_certificate_credentials_t x509_cred;
	gnutls_priority_t priority_cache;
};

void dtls_gnutls_data_destroy(struct dtls_gnutls_data *d)
{
	free(d);
}



int dtls_gnutls_read(struct conn * conn, uint8_t *buffer, int len)
{
	struct dtls_gnutls_data * d = conn->dtls_data;
	int rc = gnutls_record_recv(d->session,buffer,len);

	if ( rc == GNUTLS_E_AGAIN )
		return 0;

	if ( rc < 0 ){
		cw_log(LOG_ERR, "DTLS - read error: %s", gnutls_strerror(rc));
		conn->dtls_error=1;
		return -1;
	}

	return rc;

}

int dtls_gnutls_write(struct conn * conn, const uint8_t *buffer, int len)
{
	struct dtls_gnutls_data * d = conn->dtls_data;

	int rc = gnutls_record_send(d->session,buffer,len);

	if ( rc < 0 ){
		cw_log(LOG_ERR, "DTLS - write error: %s", gnutls_strerror(rc));
		conn->dtls_error=1;
		return -1;
	}

	return rc;
}


static int pull_timeout_func(gnutls_transport_ptr_t ptr, unsigned int ms)
{
	struct conn * conn = (struct conn*)ptr;
	time_t timer = cw_timer_start(ms/1000);
	int rc;

	uint8_t buffer[5];

	do {
		rc = conn_q_recv_packet_peek(conn,buffer,sizeof(buffer));

	}while(!cw_timer_timeout(timer) && rc==GNUTLS_E_AGAIN);

	return rc;

}

struct dtls_gnutls_data *dtls_gnutls_data_create(struct conn *conn)
{
	struct dtls_gnutls_data *d = malloc(sizeof(struct dtls_gnutls_data));
	if (!d)
		return 0;

	gnutls_certificate_allocate_credentials(&d->x509_cred);


	int rc;

	/* Set credentials */
	rc = gnutls_certificate_set_x509_key_file(d->x509_cred, conn->dtls_cert_file,
						  conn->dtls_key_file, GNUTLS_X509_FMT_PEM);

	if (rc < 0) {
		cw_log(LOG_ERR, "DTLS - Can't set cert/key: %s", gnutls_strerror(rc));
		dtls_gnutls_data_destroy(d);
		return 0;
	}

	/* Set ciphers */
	const char *errpos;
	rc = gnutls_priority_init(&d->priority_cache, conn->dtls_cipher, &errpos);
	if (rc < 0) {
		cw_log(LOG_ERR, "DTLS - Can't init ciphers '%s' at '%s' : %s", conn->dtls_cipher,
		       errpos, gnutls_strerror(rc));
		dtls_gnutls_data_destroy(d);
		return 0;
	}


	rc = gnutls_init(&d->session, GNUTLS_SERVER | GNUTLS_DATAGRAM);
	if (rc < 0) {
		cw_log(LOG_ERR, "DTLS - Can't init session: %s", gnutls_strerror(rc));
		dtls_gnutls_data_destroy(d);
		return 0;
	}

	gnutls_transport_set_ptr(d->session, conn);


	rc = gnutls_priority_set(d->session, d->priority_cache);
	if (rc < 0) {
		cw_log(LOG_ERR, "DTLS - Can't set priority: %s", gnutls_strerror(rc));
		dtls_gnutls_data_destroy(d);
		return 0;
	}


	rc = gnutls_credentials_set(d->session, GNUTLS_CRD_CERTIFICATE, d->x509_cred);

	if (rc < 0) {
		cw_log(LOG_ERR, "DTLS - Can't set credentials: %s", gnutls_strerror(rc));
		dtls_gnutls_data_destroy(d);
		return 0;
	}

	gnutls_certificate_server_set_request(d->session,GNUTLS_CERT_REQUEST);

	gnutls_transport_set_pull_function(d->session, dtls_gnutls_bio_read);
	gnutls_transport_set_push_function(d->session, dtls_gnutls_bio_write);



	return d;
}



int dtls_gnutls_accept(struct conn *conn)
{
	struct dtls_gnutls_data *d;

	gnutls_datum_t cookie_key;

	gnutls_key_generate(&cookie_key, GNUTLS_COOKIE_KEY_SIZE);
	cw_dbg(DBG_DTLS, "DTLS session cookie for %s generated: %s",
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
			/* something went wrong, log a message */
			continue;
		}
		
		rc = gnutls_dtls_cookie_verify(&cookie_key,
					       &conn->addr,
					       sizeof(conn->addr), buffer+4, tlen-4, &prestate);
		if (rc<0){
			cw_dbg(DBG_DTLS, "DTLS - Cookie couldn't be verified: %s", gnutls_strerror(rc));
			dtls_gnutls_bio_read(conn, buffer, sizeof(buffer));
			continue;
		}

		dtls_gnutls_bio_read(conn, buffer, sizeof(buffer));
		break;

	}

	if (rc <0 ){
		cw_log(LOG_ERR, "DTLS - Cookie couldn't be verified: %s", gnutls_strerror(rc));
		return 0;
	}


	cw_dbg(DBG_DTLS, "DTLS - Cookie verified! Starting handshake ...");


	d = dtls_gnutls_data_create(conn);
	if (!d)
		return 0;

	gnutls_transport_set_pull_timeout_function(d->session, pull_timeout_func);
	gnutls_dtls_prestate_set(d->session, &prestate);

	c_timer = cw_timer_start(10);
	do{
		rc = gnutls_handshake(d->session);
	}while(!cw_timer_timeout(c_timer) && rc==GNUTLS_E_AGAIN);
	


	if ( rc < 0 ) {
		cw_log(LOG_ERR, "DTLS - Error in handshake: %s", gnutls_strerror(rc));
		return 0;
	}

	//    gnutls_certificate_allocate_credentials(&x509_cred);


	cw_dbg(DBG_DTLS,"DTLS - Handshake successful");

	conn->dtls_data=d;
	conn->read = dtls_gnutls_read;
	conn->write = dtls_gnutls_write;


	return 1;
}


const char *dtls_gnutls_get_cipher(struct conn *conn)
{
	return "Unknown";
}
