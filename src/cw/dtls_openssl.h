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
 */

#ifndef __DTLS_OPENSSL_H
#define __DTLS_OPENSSL_H

#include <stdint.h>

#include <openssl/ssl.h>

#include "conn.h"

/**
 * @defgroup DTLSOPENSSL DTLS OpenSSL
 * @{
 */


struct dtls_openssl_data{
	SSL_CTX * ctx;
	SSL * ssl;
	BIO * bio;
	uint8_t buffer[2048];
	int len;
	int pos;
};

extern int dtls_openssl_init();

struct dtls_openssl_data * dtls_openssl_data_create(struct conn * conn, const SSL_METHOD * method, BIO_METHOD * bio);
extern int dtls_openssl_psk_key2bn(const char *psk_key, unsigned char *psk, unsigned int max_psk_len);
extern int dtls_openssl_generate_cookie(SSL *ssl, unsigned char *cookie, unsigned int *cookie_len);
extern int dtls_openssl_verify_cookie(SSL *ssl, unsigned char *cookie, unsigned int cookie_len);

extern int dtls_openssl_read(struct conn * conn, uint8_t *buffer, int len);
extern int dtls_openssl_write(struct conn * conn, const uint8_t *buffer, int len);
extern int dtls_openssl_connect(struct conn * conn);
extern const char * dtls_openssl_get_cipher(struct conn * conn);

extern int dtls_openssl_log_error(SSL * ssl, int rc, const char *txt);

extern int dtls_openssl_accept(struct conn * conn);
extern int dtls_openssl_set_certs(struct conn * conn, struct dtls_openssl_data *d);



extern int dtls_openssl_bio_puts(BIO *bp, const char *str);
extern int dtls_openssl_bio_read(BIO *b, char *out, int maxlen);
extern int dtls_openssl_bio_write(BIO *b, const char *data, int len);
extern long dtls_openssl_bio_ctrl(BIO *b, int cmd, long num, void *ptr);
extern int dtls_openssl_bio_new(BIO *bi);
extern int dtls_openssl_bio_free(BIO *bio);

extern const char * dtls_openssl_get_currnet_cipher();
extern int dtls_openssl_log_error_queue(const char *txt);

extern BIO_METHOD * dtls_openssl_bio_method();

extern int dtls_openssl_shutdown(struct conn *conn);

/**
 * @}
 */

#endif
