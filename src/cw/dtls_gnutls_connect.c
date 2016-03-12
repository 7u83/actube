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

#include "dtls.h"
#include "dtls_gnutls.h"

#include "conn.h"
#include "log.h"
#include "dbg.h"

#include "sock.h"
/**
 * Establish a DTLS connection using gnutls library
 * @see #dtls_connect
 */
int dtls_gnutls_connect(struct conn *conn)
{
	struct dtls_gnutls_data *d;
	d = dtls_gnutls_data_create(conn,
				    GNUTLS_CLIENT | GNUTLS_DATAGRAM | GNUTLS_NONBLOCK);

	if (!d)
		return 0;

//      gnutls_dh_set_prime_bits(d->session, 512);
/*#if GNUTLS_VERSION_NUMBER >= 0x030100
	gnutls_handshake_set_timeout(d->session, GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);

	gnutls_dtls_set_data_mtu(d->session, 1500);
#endif
	gnutls_dtls_set_mtu(d->session, 1500);
*/

	int rc;
	cw_dbg(DBG_DTLS,"Starting handshake");
	do {
		rc = gnutls_handshake(d->session);
	} while (rc == GNUTLS_E_AGAIN);


	if (rc < 0) {
		cw_log(LOG_ERR, "DTLS (gnutls) Can't connect to %s: %s",
		       sock_addr2str(&conn->addr), gnutls_strerror(rc));
		return 0;
	}


	cw_dbg(DBG_DTLS, "Handshake with %s successful", sock_addr2str(&conn->addr));

	conn->dtls_data = d;
	conn->read = dtls_gnutls_read;
	conn->write = dtls_gnutls_write;


	return 1;
}
