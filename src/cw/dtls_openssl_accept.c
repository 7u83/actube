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

#include <openssl/err.h>

#include "conn.h"
#include "sock.h"
#include "dtls_openssl.h"
#include "log.h"

int dtls_openssl_accept(struct conn *conn)
{
	if (!conn->dtls_data)
		conn->dtls_data =
		    dtls_openssl_data_create(conn, DTLSv1_server_method(),
					     dtls_openssl_bio_method());

	struct dtls_openssl_data *d = (struct dtls_openssl_data *) conn->dtls_data;
	if (!d)
		return 0;

	int i, rc;
	for (i = 0; i < conn->wait_dtls; i++) {
		rc = SSL_accept(d->ssl);
		if (rc == 1) {
			conn->read = dtls_openssl_read;
			conn->write = dtls_openssl_write;
			return 1;
		}

		rc = dtls_openssl_log_error_queue("DTLS accept error:");
		if (rc)
			return 0;
	}
	cw_log(LOG_ERR, "DTLS Error: Timeout while establishing session with %s.",
	       sock_addr2str(&conn->addr));
	return 0;
}
