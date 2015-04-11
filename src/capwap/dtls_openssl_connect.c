#include <openssl/err.h>

#include "dtls_openssl.h"
#include "log.h"
#include "dbg.h"
#include "cw_util.h"
#include "timer.h"

/*
static BIO_METHOD bio_methods = {
	BIO_TYPE_DGRAM,
	"cw packet",
	dtls_openssl_bio_write,
	dtls_openssl_bio_read,
	dtls_openssl_bio_puts,
	NULL, 			// dgram_gets
	dtls_openssl_bio_ctrl,
	dtls_openssl_bio_new,
	dtls_openssl_bio_free,
	NULL,
};
*/

 unsigned int psk_client_cb(SSL * ssl,
				  const char *hint,
				  char *identity,
				  unsigned int max_identity_len,
				  unsigned char *psk, unsigned int max_psk_len)
{

	BIO *b = SSL_get_rbio(ssl);
	struct conn *conn = b->ptr;

//      printf("KEYY: %s\n",conn->dtls_psk);

	snprintf(identity, max_identity_len, "CLient_identity");

	int l = conn->dtls_psk_len < max_psk_len ? conn->dtls_psk_len : max_psk_len;
	memcpy(psk, conn->dtls_psk, l);
	return l;

}


int dtls_openssl_connect(struct conn *conn)
{
	if (!conn->dtls_data)
		conn->dtls_data =
		    dtls_openssl_data_create(conn, DTLSv1_client_method(),
					     dtls_openssl_bio_method());

	struct dtls_openssl_data *d = (struct dtls_openssl_data *) conn->dtls_data;
	if (!d)
		return 0;

//	if (conn->dtls_psk)
//		SSL_set_psk_client_callback(d->ssl, psk_client_cb);

	int rc;

	errno =0;
	time_t timer = cw_timer_start(10);
	do {
		rc = SSL_connect(d->ssl);
	}while(rc!=1 && errno==EAGAIN && !cw_timer_timeout(timer));
	

	if (rc == 1) {
		cw_dbg(DBG_DTLS,"SSL connect successfull!");
		conn->read = dtls_openssl_read;
		conn->write = dtls_openssl_write;
		return 1;
	}

	rc = dtls_openssl_log_error(0, rc, "DTLS connect");




	return 0;
}
