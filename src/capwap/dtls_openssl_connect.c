#include <openssl/err.h>

#include "dtls_openssl.h"
#include "cw_log.h"

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


//      dtls_openssl_set_certs(conn,d);

	if (conn->dtls_cert_file && conn->dtls_key_file) {
	}
//      char errstr[256];
	int rc;
//      do{ 

printf("Calling SSL connect\n");
	rc = SSL_connect(d->ssl);
printf("Back from SSL connect\n");
exit(0);
/*

		if (rc <= 0){
			int e;
			e = SSL_get_error(d->ssl,rc);
			printf("Get Err %i\n",e);


			printf("SSL Connect rc = %i\n",rc);
			e = ERR_get_error();
			ERR_error_string(e,errstr);
			cw_log(LOG_ERR,"SSL_connect %s",errstr);

			


		}

*/

	if (rc == 1) {
		conn->read = dtls_openssl_read;
		conn->write = dtls_openssl_write;
		return 1;
	}

//              rc = dtls_openssl_log_error(d->ssl,rc,"DTLS connect");
	rc = dtls_openssl_log_error(0, rc, "DTLS connect");

//      } while (rc == 0 );

//      printf("DTLS connect RC: %i\n",rc);

//      int e = ERR_get_error();
//      ERR_error_string(e,errstr);
//      cw_log(LOG_ERR,"SSL_accept %s",errstr);


	return 0;
}
