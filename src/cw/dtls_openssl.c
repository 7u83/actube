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
#include "capwap.h"

#include "dbg.h"
#include "log.h"

#include "cw_util.h"
#include "rand.h"

#include "conn.h"
#include "sock.h"


#ifdef WITH_CW_LOG_DEBUG
static const char * ssl_version2str(int version)
{
	switch(version){
		case SSL2_VERSION:
			return "SSLv2";
		case SSL3_VERSION:
			return "SSLv3";
		case TLS1_VERSION:
			return "TLSv1";
		case DTLS1_VERSION:
			return "DTLSv1";
//		case DTLS1_2_VERSION:
//			return "DTLSv1.2";
	}
	return "Version unknown";
}

static void dtls_debug_cb(int write_p,int version,int type, const void * buf,size_t len, SSL * ssl, void * arg)
{
	char buffer[200];
	char * s = buffer;

	if (write_p)
		s += sprintf(s,"SSL msg out: ");
	else
		s += sprintf(s,"SSL msg in: ");

	s+=sprintf(s,"type = %d (0x%02X), %s (%08x), len = %d",type,type,ssl_version2str(version),version,(int)len);
//	cw_dbg(DBG_DTLS_DETAIL,buffer);
}
#endif


static void dtls_info_cb (const SSL *ssl, int where, int ret)
{
	const char *str = NULL;

	str = where & SSL_ST_CONNECT ? "connect" : where & SSL_ST_ACCEPT ? "accept" : "undefined";
	if (where & SSL_CB_LOOP)
	{
		cw_dbg (DBG_DTLS_DETAIL,"SSL state [\"%s\"]: %s", str, SSL_state_string_long (ssl));
	}
	else if (where & SSL_CB_ALERT)
	{
		cw_dbg (DBG_DTLS_DETAIL,"SSL: alert [\"%s\"]: %s : %s", where & SSL_CB_READ ? "read" : "write", \
			SSL_alert_type_string_long (ret), SSL_alert_desc_string_long (ret));
	}
}

int pem_passwd_cb(char *buf, int size, int rwflag, void *password)
{
	if (!password){
		cw_dbg(DBG_DTLS, "DTLS - No password given to decrypt privat key");
		return 0;
	}

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
	const char * version = SSLeay_version(SSLEAY_VERSION);
	cw_dbg(DBG_INFO,"Init SSL library - %s",version);
	int rc = SSL_library_init();
	ERR_clear_error();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();
	return rc;
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


	/*	cw_dbg(DBG_DTLS,"DTLS - Using key file %s",conn->dtls_key_file); */
		rc = SSL_CTX_use_PrivateKey_file(d->ctx,conn->dtls_key_file,SSL_FILETYPE_PEM);
		if (!rc){

			dtls_openssl_log_error(0,rc,"DTLS certificate error:");
			dtls_openssl_data_destroy(d);	
			return 0;
		}

	/*	cw_dbg(DBG_DTLS,"DTLS - Using cert file %s",conn->dtls_cert_file); */

/*		rc = SSL_CTX_use_certificate_file(d->ctx,conn->dtls_cert_file,SSL_FILETYPE_PEM);*/

		rc = SSL_CTX_use_certificate_chain_file(d->ctx,conn->dtls_cert_file);
		if (!rc){

			dtls_openssl_log_error(0,rc,"DTLS certificate error:");
			dtls_openssl_data_destroy(d);	
			return 0;
		}

	}

	return 1;
}


int generate_session_id(const SSL *ssl, unsigned char * id, unsigned int *id_len)
{
/*	BIO * b = SSL_get_rbio(ssl);
	struct conn * conn = b->ptr;
*/


	printf ("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMaking session id\n");
	const char * sessid = "9123456789";
	memcpy(id,sessid,strlen(sessid));
	*id_len=strlen(sessid);
	return 1;
}



static int dtls_verify_peer_callback (int ok, X509_STORE_CTX *ctx) 
{
	printf ("Verify callback called with ok = %d\n",ok);

//	SSL *ssl;
//	ssl = X509_STORE_CTX_get_ex_data(ctx, SSL_get_ex_data_X509_STORE_CTX_idx());

	char buf[1024];
	X509   *err_cert;

	err_cert = X509_STORE_CTX_get_current_cert(ctx);
	X509_NAME_oneline(X509_get_subject_name(err_cert), buf, 256);

	printf("Err cert: %s\n",buf);

return 1;
	exit(0);
	return 1;
}




static unsigned int psk_server_cb(SSL *ssl,const char *identity, unsigned char * psk, unsigned int max_psk_len)
{
	BIO * b = SSL_get_rbio(ssl);
	struct conn * conn = b->ptr;
	int l = conn->dtls_psk_len < max_psk_len ? conn->dtls_psk_len : max_psk_len;
	memcpy(psk,conn->dtls_psk,l);
	return l;
}






struct dtls_openssl_data * dtls_openssl_data_create(struct conn * conn, const SSL_METHOD * method, BIO_METHOD * bio)
{
	struct dtls_openssl_data * d = malloc(sizeof(struct dtls_openssl_data));
	if (!d)
		return NULL;
	memset(d,0,sizeof(struct dtls_openssl_data));

	/* create new SSL ctx. (In future this should not be done for every connection) */
	d->ctx = SSL_CTX_new(method);
	if (!d->ctx){
		dtls_openssl_data_destroy(d);	
		return NULL;
	}

	
	int rc = SSL_CTX_set_cipher_list(d->ctx, conn->dtls_cipher);
	if (!rc){
		dtls_openssl_log_error(0,rc,"DTLS setup cipher error:");
		dtls_openssl_data_destroy(d);	
		return 0;
	}

	/* set dtls psk if exists */
	if (conn->dtls_psk)
		SSL_CTX_set_psk_server_callback( d->ctx, psk_server_cb);



	/* setup certificates */
	rc = dtls_openssl_set_certs(conn,d);
	if (!rc)
		return 0;


	/* enable or disable peer verfifying */
	if (!conn->dtls_verify_peer){
		cw_dbg(DBG_DTLS, "DTLS - verify peer is turned off");
		SSL_CTX_set_verify(d->ctx, SSL_VERIFY_PEER, dtls_verify_peer_callback);
	}
	else{
		/* In case of verify peer is on we let the ssl library do the verificatoin */
		SSL_CTX_set_verify(d->ctx, SSL_VERIFY_PEER, NULL);
	}

	/* setup cookie handling */
	SSL_CTX_set_options(d->ctx, SSL_OP_COOKIE_EXCHANGE); 
	SSL_CTX_set_cookie_generate_cb(d->ctx, dtls_openssl_generate_cookie);
	SSL_CTX_set_cookie_verify_cb(d->ctx, dtls_openssl_verify_cookie);


	/* setup debugging */
#ifdef WITH_CW_LOG_DEBUG
	SSL_CTX_set_msg_callback(d->ctx,dtls_debug_cb);
	SSL_CTX_set_info_callback (d->ctx, dtls_info_cb);
	
#endif



	SSL_CTX_set_read_ahead(d->ctx, 1);





	rc = SSL_CTX_load_verify_locations(d->ctx,"../../ssl/root-ca.pem",NULL);

	


//	SSL_CTX_set_session_cache_mode(d->ctx, SSL_SESS_CACHE_BOTH);
	SSL_CTX_set_options(d->ctx, SSL_OP_NO_SSLv2 |SSL_OP_NO_SSLv3 );
//	SSL_CTX_set_generate_session_id(d->ctx,generate_session_id);


	SSL_CTX_set_timeout(d->ctx,30);

//	rc =SSL_CTX_set_max_proto_version (d->ctx,DTLS1_VERSION);
//	printf("MAXMAMX = %d\n",rc);


//	SSL_CTX_set_verify(d->ctx, SSL_VERIFY_PEER | SSL_VERIFY_CLIENT_ONCE, dtls_verify_callback);
//	SSL_CTX_set_verify(d->ctx, SSL_VERIFY_PEER, dtls_verify_callback);

//	SSL_CTX_set_tmp_rsa_callback(d->ctx,tmp_rsa_callback);

//	SSL_CTX_set_mode(d->ctx,SSL_MODE_SEND_SERVERHELLO_TIME);





// rsa_512 = RSA_generate_key(512,RSA_F4,NULL,NULL);
// if (rsa_512 == NULL)
//     evaluate_error_queue();

// rsa_1024 = RSA_generate_key(1024,RSA_F4,NULL,NULL);
// if (rsa_1024 == NULL)
//   evaluate_error_queue();


//	printf ("Ver cookie rc %d\n",rc);


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
 * Convert the PSK key (psk_key) from ascii to binary (psk).
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

	if (psk_len <= 0)	
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













//int cookie_initialized=0;
//#define COOKIE_SECRET_LENGTH 16
//unsigned char cookie_secret[COOKIE_SECRET_LENGTH];

int dtls_openssl_generate_cookie(SSL *ssl, unsigned char *cookie, unsigned int *cookie_len)
{
	
	BIO * b = SSL_get_rbio(ssl);
	struct conn * conn = b->ptr;

	cw_rand(conn->dtls_cookie,sizeof(conn->dtls_cookie));

	/* we "missuse" sockaddr2str to convert our cookie to a hex str */
	cw_dbg(DBG_DTLS,"DTLS session cookie for %s generated: %s",
		sock_addr2str(&conn->addr), sock_hwaddr2idstr(conn->dtls_cookie,sizeof(conn->dtls_cookie)));

	memcpy(cookie,conn->dtls_cookie,sizeof(conn->dtls_cookie));
	*cookie_len=sizeof(conn->dtls_cookie);
	return 1;

}




int dtls_openssl_verify_cookie(SSL *ssl, unsigned char *cookie, unsigned int len)
{
	BIO * b = SSL_get_rbio(ssl);
	struct conn * conn = b->ptr;

	cw_dbg(DBG_DTLS,"Verifying DTLS cookie from %s: %s",
		sock_addr2str(&conn->addr),sock_hwaddr2idstr(conn->dtls_cookie,len));

	if (len != sizeof(conn->dtls_cookie)){
		return 0;
	}

	if (memcmp(cookie,conn->dtls_cookie,len)!=0){
		return 0;
	}
	return 1;
}


int dtls_openssl_read(struct conn * conn, uint8_t *buffer, int len)
{
	struct dtls_openssl_data * d = conn->dtls_data;
	int rc = SSL_read(d->ssl,buffer,len);
	if (dtls_openssl_log_error_queue("DTLS read error:")){
		conn->dtls_error=1;
		return -1;
	}
	return rc;
}

int dtls_openssl_write(struct conn * conn, const uint8_t *buffer, int len)
{
	struct dtls_openssl_data * d = conn->dtls_data;
	int rc = SSL_write(d->ssl,buffer,len);
	if (dtls_openssl_log_error_queue("DTLS write error:")){
		conn->dtls_error=1;
		return -1;
	}
	return rc;
}

