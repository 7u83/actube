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

/*
 * GnuTLS specific definitions for DTLS
 */ 

#ifndef __DTLS_GNUTLS_H
#define __DTLS_GNUTLS_H

#include <gnutls/gnutls.h>

#include "conn.h"

/* "public" functions */
extern int dtls_gnutls_init();
extern int dtls_gnutls_accept(struct conn * conn);
extern int dtls_gnutls_connect(struct conn * conn);
extern const char * dtls_gnutls_get_cipher(struct conn * conn);
struct dtls_ssl_cert dtls_gnutls_get_peers_cert(struct conn * conn,unsigned int n);
extern int dtls_gnutls_shutdown(struct conn *conn);


/* functions used only by capwap libray */

extern ssize_t dtls_gnutls_bio_read(gnutls_transport_ptr_t b, void *out, size_t maxlen);
extern ssize_t dtls_gnutls_bio_write(gnutls_transport_ptr_t b, const void *data, size_t len);
extern int dtls_gnutls_bio_wait(gnutls_transport_ptr_t ptr, unsigned int ms);
extern int dtls_gnutls_read(struct conn * conn, uint8_t *buffer, int len);
extern int dtls_gnutls_write(struct conn * conn, const uint8_t *buffer, int len);

struct dtls_gnutls_data {
	gnutls_session_t session;
	gnutls_certificate_credentials_t x509_cred;
	gnutls_priority_t priority_cache;
	gnutls_dh_params_t dh_params;
};

struct dtls_gnutls_data *dtls_gnutls_data_create(struct conn *conn,int config);

#endif

