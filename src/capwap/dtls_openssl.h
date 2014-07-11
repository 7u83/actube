#ifndef __DTLS_OPENSLL_H
#define __DTLS_OPENSSL_H

#include <stdint.h>

#include <openssl/ssl.h>

#include "conn.h"

struct dtls_openssl_data{
	SSL_CTX * ctx;
	SSL * ssl;
	BIO * bio;

	uint8_t buffer[2048];
	int len;
	int pos;
};

extern int dtls_openssl_init();
//extern struct dtls_openssl_data * dtls_openssl_data_create(const SSL_METHOD * method );
//struct dtls_openssl_data * dtls_openssl_data_create(const SSL_METHOD * method, const BIO_METHOD * bio);
struct dtls_openssl_data * dtls_openssl_data_create(struct conn * conn, const SSL_METHOD * method, BIO_METHOD * bio);
extern int dtls_openssl_psk_key2bn(const char *psk_key, unsigned char *psk, unsigned int max_psk_len);
extern int dtls_openssl_generate_cookie(SSL *ssl, unsigned char *cookie, unsigned int *cookie_len);
extern int dtls_openssl_verify_cookie(SSL *ssl, unsigned char *cookie, unsigned int cookie_len);

extern int dtls_openssl_read(struct conn * conn, uint8_t *buffer, int len);
extern int dtls_openssl_write(struct conn * conn, const uint8_t *buffer, int len);
extern int dtls_openssl_connect(struct conn * conn);

extern int dtls_openssl_log_error(SSL * ssl, int rc, const char *txt);




extern int dtls_openssl_bio_puts(BIO *bp, const char *str);
extern int dtls_openssl_bio_read(BIO *b, char *out, int maxlen);
extern int dtls_openssl_bio_write(BIO *b, const char *data, int len);
extern long dtls_openssl_bio_ctrl(BIO *b, int cmd, long num, void *ptr);
extern int dtls_openssl_bio_new(BIO *bi);
extern int dtls_openssl_bio_free(BIO *bio);


extern BIO *bbb;


#endif
