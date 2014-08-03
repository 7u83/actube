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
#include <errno.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>


#include "dtls_openssl.h"

#include "cw_log.h"
#include "conn.h"


int pem_passwd_cb(char *buf, int size, int rwflag, void *password)
{
         strncpy(buf, (char *)(password), size);
         buf[size - 1] = '\0';
         return(strlen(buf));
}

 /* Set up ephemeral RSA stuff */
 RSA *rsa_512 = NULL;
 RSA *rsa_1024 = NULL;



 RSA *tmp_rsa_callback(SSL *s, int is_export, int keylength)
 {
    RSA *rsa_tmp=NULL;

    switch (keylength) {
    case 512:
      if (rsa_512)
        rsa_tmp = rsa_512;
      else { /* generate on the fly, should not happen in this example */
        rsa_tmp = RSA_generate_key(keylength,RSA_F4,NULL,NULL);
        rsa_512 = rsa_tmp; /* Remember for later reuse */
      }
      break;
    case 1024:
      if (rsa_1024)
        rsa_tmp=rsa_1024;
      else
        exit(0); //should_not_happen_in_this_example();
      break;
    default:
      /* Generating a key on the fly is very costly, so use what is there */
      if (rsa_1024)
        rsa_tmp=rsa_1024;
      else
        rsa_tmp=rsa_512; /* Use at least a shorter key */
    }
    return(rsa_tmp);
 }





int dtls_openssl_init()
{
	cw_log_debug0("Init ssl library");
	SSL_load_error_strings();
	return SSL_library_init();
}


int dtls_openssl_log_error_queue(const char *txt)
{
	int e = ERR_get_error();
	if (e==0)
		return 0;

	char errstr[256];
	while (e!=0){
		ERR_error_string(e,errstr);
		cw_log(LOG_ERR,"%s - %s",txt,errstr);
		e = ERR_get_error();
	}
	return 1;
}

int dtls_openssl_log_error(SSL * ssl, int rc, const char *txt)
{
	int en = errno; /* save errno */

	if (!ssl){
		return dtls_openssl_log_error_queue(txt);
	}

	int e;
	e = SSL_get_error(ssl,rc);

	char errstr[256];
	ERR_error_string(e,errstr);
	cw_log(LOG_ERR,"%s - %s","SSSSS",errstr);

	switch (e){
		case SSL_ERROR_ZERO_RETURN:
			break;
		case SSL_ERROR_SYSCALL:
			if (!dtls_openssl_log_error_queue(txt)){
				/* error queu was empty */
				if (rc<0){
					cw_log(LOG_ERR,"%s - %s",txt,strerror(en));
					return 1;
				}
				
				cw_log(LOG_ERR,"%s - EOF observed",txt);
				return 1;
			}
			

	}	
	return 0;
}


void dtls_openssl_data_destroy(struct dtls_openssl_data * d){
	if (!d)
		return;

	if (d->ssl)
		SSL_free(d->ssl);

	if (d->ctx)
		SSL_CTX_free(d->ctx);


	free(d);
}

int dtls_openssl_set_certs(struct conn * conn, struct dtls_openssl_data *d)
{
	int rc;
	if (conn->dtls_key_file && conn->dtls_cert_file){
		SSL_CTX_set_default_passwd_cb_userdata(d->ctx, conn->dtls_key_pass);
		SSL_CTX_set_default_passwd_cb(d->ctx, pem_passwd_cb);


		cw_log_debug1("DTLS - Setting key file %s",conn->dtls_key_file);
		rc = SSL_CTX_use_PrivateKey_file(d->ctx,conn->dtls_key_file,SSL_FILETYPE_PEM);
		if (!rc){

			dtls_openssl_log_error(0,rc,"DTLS:");
			dtls_openssl_data_destroy(d);	
			return 0;
		}

		cw_log_debug1("DTLS - Setting cert file %s",conn->dtls_cert_file);
		rc = SSL_CTX_use_certificate_file(d->ctx,conn->dtls_cert_file,SSL_FILETYPE_PEM);
		if (!rc){

			dtls_openssl_log_error(0,rc,"DTLS:");
			dtls_openssl_data_destroy(d);	
			return 0;
		}

	}

	return 1;
}


int dtls_verify_callback (int ok, X509_STORE_CTX *ctx) {


	printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX501 verify\n");

	/* This function should ask the user
	 * if he trusts the received certificate.
	 * Here we always trust.
	 */
	return 1;
}


struct dtls_openssl_data * dtls_openssl_data_create(struct conn * conn, const SSL_METHOD * method, BIO_METHOD * bio)
{
	struct dtls_openssl_data * d = malloc(sizeof(struct dtls_openssl_data));
	if (!d)
		return 0;
	memset(d,0,sizeof(struct dtls_openssl_data));

	d->ctx = SSL_CTX_new(method);
	if (!d->ctx){
		dtls_openssl_data_destroy(d);	
		return 0;
	}

	SSL_CTX_set_read_ahead(d->ctx, 1);
	
	int rc = SSL_CTX_set_cipher_list(d->ctx, conn->dtls_cipher);
	if (!rc){
		dtls_openssl_log_error(0,rc,"DTLS:");
		dtls_openssl_data_destroy(d);	
		return 0;
	}

	SSL_CTX_set_session_cache_mode(d->ctx, SSL_SESS_CACHE_OFF);
	SSL_CTX_set_options(d->ctx, SSL_OP_COOKIE_EXCHANGE);

	SSL_CTX_set_cookie_generate_cb(d->ctx, dtls_openssl_generate_cookie);
	SSL_CTX_set_cookie_verify_cb(d->ctx, dtls_openssl_verify_cookie);

	SSL_CTX_set_verify(d->ctx, SSL_VERIFY_PEER | SSL_VERIFY_CLIENT_ONCE, dtls_verify_callback);

	SSL_CTX_set_tmp_rsa_callback(d->ctx,tmp_rsa_callback);


 rsa_512 = RSA_generate_key(512,RSA_F4,NULL,NULL);
// if (rsa_512 == NULL)
//     evaluate_error_queue();

 rsa_1024 = RSA_generate_key(1024,RSA_F4,NULL,NULL);
// if (rsa_1024 == NULL)
//   evaluate_error_queue();


	printf ("Ver cookie rc %d\n",rc);


/*
	if (conn->dtls_key_file && conn->dtls_cert_file){
		SSL_CTX_set_default_passwd_cb_userdata(d->ctx, conn->dtls_key_pass);
		SSL_CTX_set_default_passwd_cb(d->ctx, pem_passwd_cb);


		cw_log_debug1("DTLS - Setting key file %s",conn->dtls_key_file);
		rc = SSL_CTX_use_PrivateKey_file(d->ctx,conn->dtls_key_file,SSL_FILETYPE_PEM);
		if (!rc){

			dtls_openssl_log_error(0,rc,"DTLS:");
			dtls_openssl_data_destroy(d);	
			return 0;
		}

		cw_log_debug1("DTLS - Setting cert file %s",conn->dtls_cert_file);
		rc = SSL_CTX_use_certificate_file(d->ctx,conn->dtls_cert_file,SSL_FILETYPE_PEM);
		if (!rc){

			dtls_openssl_log_error(0,rc,"DTLS:");
			dtls_openssl_data_destroy(d);	
			return 0;
		}



	}

*/
	rc = dtls_openssl_set_certs(conn,d);
	if (!rc)
		return 0;



	d->ssl = SSL_new(d->ctx);
	if (!d->ssl){
		dtls_openssl_data_destroy(d);
		return 0;
	}

	
	d->bio = BIO_new(bio);
	d->bio->ptr = conn;
	SSL_set_bio(d->ssl, d->bio, d->bio);

	return d;
}


/* 
 * Convert the PSK key (psk_key) in ascii to binary (psk).
 */
int dtls_openssl_psk_key2bn(const char *psk_key, unsigned char *psk, unsigned int max_psk_len) {

	unsigned int psk_len = 0;
	int ret;
	BIGNUM *bn = NULL;

	ret = BN_hex2bn(&bn, psk_key);
	if (!ret) {

		cw_log(LOG_ERR,"Could not convert PSK key '%s' to BIGNUM\n", psk_key);
		if (bn)
            		BN_free(bn);
        	return 0;
        }

	if (BN_num_bytes(bn) > max_psk_len) {

		cw_log(LOG_ERR,"psk buffer of callback is too small (%d) for key (%d)\n",
			max_psk_len, BN_num_bytes(bn));
		BN_free(bn);
		return 0;
	}
	psk_len = BN_bn2bin(bn, psk);
	BN_free(bn);

	if (psk_len < 0)	
		goto out_err;
	return psk_len;
out_err:
	return 0;
}


#include <arpa/inet.h>
//#include <socket.h>
#include <netinet/in.h>

int dtls_openssl_shutdown(struct conn *conn)
{
	conn->write = conn->send_packet;
	conn->read = conn->recv_packet;

	struct dtls_openssl_data * d = (struct dtls_openssl_data*)conn->dtls_data;
	if (!d)
		return 0;





	SSL_shutdown(d->ssl);
	dtls_openssl_data_destroy(d);

	conn->dtls_data=0;

	return 1;
}
















int cookie_initialized=0;
#define COOKIE_SECRET_LENGTH 16
unsigned char cookie_secret[COOKIE_SECRET_LENGTH];

int dtls_openssl_generate_cookie(SSL *ssl, unsigned char *cookie, unsigned int *cookie_len)
{
	

printf(" Gen cookie!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

	const char * coo = "tube7u83";
	memcpy(cookie,coo,strlen(coo));
	*cookie_len=strlen(coo);
	return 1;


	unsigned char *buffer, result[EVP_MAX_MD_SIZE];
	unsigned int length = 0, resultlength;
	union {
		struct sockaddr_storage ss;
		struct sockaddr_in6 s6;
		struct sockaddr_in s4;
	} peer;

	/* Initialize a random secret */
	if (!cookie_initialized)
	{
		if (!RAND_bytes(cookie_secret, COOKIE_SECRET_LENGTH))
		{
			printf("error setting random cookie secret\n");
			return 0;
		}
		cookie_initialized = 1;
	}


	return 1;

	/* Read peer information */
	(void) BIO_dgram_get_peer(SSL_get_rbio(ssl), &peer);

	/* Create buffer with peer's address and port */
	length = 0;
	switch (peer.ss.ss_family) {
		case AF_INET:
			length += sizeof(struct in_addr);
			break;
		case AF_INET6:
			length += sizeof(struct in6_addr);
			break;
		default:
			OPENSSL_assert(0);
			break;
	}
	length += sizeof(in_port_t);
	buffer = (unsigned char*) OPENSSL_malloc(length);

	if (buffer == NULL) {
		printf("out of memory\n");
		return 0;
	}

	switch (peer.ss.ss_family) {
		case AF_INET:
			memcpy(buffer,
			       &peer.s4.sin_port,
			       sizeof(in_port_t));
			memcpy(buffer + sizeof(peer.s4.sin_port),
			       &peer.s4.sin_addr,
			       sizeof(struct in_addr));
			break;
		case AF_INET6:
			memcpy(buffer,
			       &peer.s6.sin6_port,
			       sizeof(in_port_t));
			memcpy(buffer + sizeof(in_port_t),
			       &peer.s6.sin6_addr,
			       sizeof(struct in6_addr));
			break;
		default:
			OPENSSL_assert(0);
			break;
	}

	/* Calculate HMAC of buffer using the secret */
	HMAC(EVP_sha1(), (const void*) cookie_secret, COOKIE_SECRET_LENGTH,
	     (const unsigned char*) buffer, length, result, &resultlength);
	OPENSSL_free(buffer);

	memcpy(cookie, result, resultlength);
	*cookie_len = resultlength;

	return 1;
}




int dtls_openssl_verify_cookie(SSL *ssl, unsigned char *cookie, unsigned int cookie_len)
{

printf(" Verify cookie!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	char t[400];
	strncpy(t,(char*)cookie,cookie_len);
	t[cookie_len]=0;
	printf("TCoo: %s\n",t);

	return 1;

	unsigned char *buffer, result[EVP_MAX_MD_SIZE];
	unsigned int length = 0, resultlength;
	union {
		struct sockaddr_storage ss;
		struct sockaddr_in6 s6;
		struct sockaddr_in s4;
	} peer;

	/* If secret isn't initialized yet, the cookie can't be valid */
	if (!cookie_initialized)
		return 0;

	/* Read peer information */
	(void) BIO_dgram_get_peer(SSL_get_rbio(ssl), &peer);

	/* Create buffer with peer's address and port */
	length = 0;
	switch (peer.ss.ss_family) {
		case AF_INET:
			length += sizeof(struct in_addr);
			break;
		case AF_INET6:
			length += sizeof(struct in6_addr);
			break;
		default:
			OPENSSL_assert(0);
			break;
	}
	length += sizeof(in_port_t);
	buffer = (unsigned char*) OPENSSL_malloc(length);

	if (buffer == NULL){
		printf("out of memory\n");
		return 0;
	}

	switch (peer.ss.ss_family) {
		case AF_INET:
			memcpy(buffer,
			       &peer.s4.sin_port,
			       sizeof(in_port_t));
			memcpy(buffer + sizeof(in_port_t),
			       &peer.s4.sin_addr,
			       sizeof(struct in_addr));
			break;
		case AF_INET6:
			memcpy(buffer,
			       &peer.s6.sin6_port,
			       sizeof(in_port_t));
			memcpy(buffer + sizeof(in_port_t),
			       &peer.s6.sin6_addr,
			       sizeof(struct in6_addr));
			break;
		default:
			OPENSSL_assert(0);
			break;
	}

	/* Calculate HMAC of buffer using the secret */
	HMAC(EVP_sha1(), (const void*) cookie_secret, COOKIE_SECRET_LENGTH,
	     (const unsigned char*) buffer, length, result, &resultlength);
	OPENSSL_free(buffer);

	if (cookie_len == resultlength && memcmp(result, cookie, resultlength) == 0)
		return 1;

	return 0;
	}


/*
struct pass_info {
	union {
		struct sockaddr_storage ss;
		struct sockaddr_in6 s6;
		struct sockaddr_in s4;
	} server_addr, client_addr;
	SSL *ssl;
};
*/


int dtls_openssl_read(struct conn * conn, uint8_t *buffer, int len)
{
	struct dtls_openssl_data * d = conn->dtls_data;
	return SSL_read(d->ssl,buffer,len);
}

int dtls_openssl_write(struct conn * conn, const uint8_t *buffer, int len)
{
	struct dtls_openssl_data * d = conn->dtls_data;
	return SSL_write(d->ssl,buffer,len);
}

