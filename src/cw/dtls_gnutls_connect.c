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
 * @brief Implements dtls_gnutls_connect
 */

#include "gnutls/dtls.h"


#include "dtls_common.h"
#include "dtls_gnutls.h"

#include "conn.h"
#include "log.h"
#include "dbg.h"

#include "sock.h"
/**
 * Establish a DTLS connection using gnutls library
 * @see #dtls_connec
 */
int dtls_gnutls_connect(struct conn *conn)
{
	int rc;
	char sock_buf[SOCK_ADDR_BUFSIZE];
	struct dtls_gnutls_data *d;
	gnutls_datum_t key;
	d = dtls_gnutls_data_create(conn,
				    GNUTLS_CLIENT | GNUTLS_DATAGRAM | GNUTLS_NONBLOCK);

	if (!d)
		return 0;

/*//      gnutls_dh_set_prime_bits(d->session, 512);*/
/*#if GNUTLS_VERSION_NUMBER >= 0x030100
	gnutls_handshake_set_timeout(d->session, GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);

	gnutls_dtls_set_data_mtu(d->session, 1500);
#endif
	gnutls_dtls_set_mtu(d->session, 1500);
*/

/*
	if (conn->dtls_psk != NULL){
		key.data=(unsigned char*)conn->dtls_psk;
		key.size=strlen(conn->dtls_psk);
	
		rc = gnutls_credentials_set(d->session, GNUTLS_CRD_PSK, &key);
		if (rc<0) {
			cw_log(LOG_ERR, "DTLS - Can't set x.509 credentials: %s", gnutls_strerror(rc));
			dtls_gnutls_data_destroy(d);
			return 0;

		}
	}

*/


	if (conn->dtls_psk_enable){
		gnutls_psk_client_credentials_t cred;
		rc = gnutls_psk_allocate_client_credentials(&cred);
		if (rc != 0) {
			cw_dbg(DBG_DTLS,"gnutls_psk_allocate_client_credentials() failed.\n");
			return 0;
		}
		
		
		key.size = bstr16_len(conn->dtls_psk);
		key.data = bstr16_data(conn->dtls_psk);
		
		
		/* Put the username and key into the structure we use to tell GnuTLs what
		// the credentials are. The example server doesn't care about usernames, so
		// we use "Alice" here.*/
		rc = gnutls_psk_set_client_credentials(cred, "Alice", &key, GNUTLS_PSK_KEY_RAW);
		rc = gnutls_credentials_set(d->session, GNUTLS_CRD_PSK, cred);
		if (rc != 0) {
			cw_log(LOG_ERR,"gnutls_credentials_set() failed.");
		}

	
	
	}



	cw_dbg(DBG_DTLS,"Starting handshake");
	do {
		rc = gnutls_handshake(d->session);
	} while (rc == GNUTLS_E_AGAIN);


	if (rc < 0) {
		cw_log(LOG_ERR, "DTLS (gnutls) Can't connect to %s: %s",
		       sock_addr2str(&conn->addr,sock_buf), gnutls_strerror(rc));
		return 0;
	}


	cw_dbg(DBG_DTLS, "Handshake with %s successful", sock_addr2str(&conn->addr,sock_buf));

	conn->dtls_data = d;
	conn->read = dtls_gnutls_read;
	conn->write = dtls_gnutls_write;


	return 1;
}
