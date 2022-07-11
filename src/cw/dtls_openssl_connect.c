#include <openssl/err.h>


#include "dtls_openssl.h"
#include "log.h"
#include "dbg.h"
#include "cw_util.h"
#include "timer.h"

 unsigned int psk_client_cb(SSL * ssl,
				  const char *hint,
				  char *identity,
				  unsigned int max_identity_len,
				  unsigned char *psk, unsigned int max_psk_len)
{
	int l;
	BIO *b = SSL_get_rbio(ssl);
	/*struct conn *conn = b->ptr;*/
	struct conn * conn = BIO_get_data(b); /*b->ptr;*/

	snprintf(identity, max_identity_len, "CLient_identity");

	l = bstr16_len(conn->dtls_psk) < max_psk_len ? bstr16_len(conn->dtls_psk) : max_psk_len;
	memcpy(psk, bstr16_data(conn->dtls_psk), l);
	return l;

}


int 
dtls_openssl_connect(struct conn *conn)
{
	struct dtls_openssl_data *d;
	int rc;
	time_t timer;

	cw_dbg(DBG_DTLS_BIO, "DTLS Connect call");

	BIO_METHOD * biomethod = dtls_openssl_bio_method();
	if (!biomethod){
		cw_dbg(DBG_DTLS_BIO, "ERROR: Creating new OpenSSL BIO");
		return 0;
	}

	cw_dbg(DBG_DTLS_BIO, "DTLS Connect call 1");
	
	if (!conn->dtls_data)
		conn->dtls_data =
		    dtls_openssl_data_create(conn, DTLSv1_client_method(),
					     biomethod);

	cw_dbg(DBG_DTLS_BIO, "DTLS Connect call 2");

	d = (struct dtls_openssl_data *) conn->dtls_data;
	if (!d)
		return 0;

/*
	if (conn->dtls_psk)
		SSL_set_psk_client_callback(d->ssl, psk_client_cb);
*/

	cw_dbg(DBG_DTLS_BIO, "DTLS Connect call 3");

	errno =0;
	timer = cw_timer_start(10);
	do {
		rc = SSL_connect(d->ssl);
	}while(rc!=1 && errno==EAGAIN && !cw_timer_timeout(timer));

	cw_dbg(DBG_DTLS_BIO, "DTLS Connect call 4");

	if (rc == 1) {
		cw_dbg(DBG_DTLS,"SSL connect successfull!");
		conn->read = dtls_openssl_read;
		conn->write = dtls_openssl_write;
		return 1;
	}
	cw_dbg(DBG_DTLS_BIO, "DTLS Connect call 5");

	rc = dtls_openssl_log_error(d->ssl, rc, "DTLS connect");
	return 0;
}

