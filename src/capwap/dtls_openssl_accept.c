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

#include "dtls_openssl.h"
#include "conn.h"

#include "cw_log.h"



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



static unsigned int psk_server_cb(SSL *ssl,const char *identity, unsigned char * psk, unsigned int max_psk_len)
{
	BIO * b = SSL_get_rbio(ssl);
	struct conn * conn = b->ptr;
	int l = conn->dtls_psk_len < max_psk_len ? conn->dtls_psk_len : max_psk_len;
	memcpy(psk,conn->dtls_psk,l);
	return l;
}



int dtls_openssl_accept(struct conn * conn)
{
	if (!conn->dtls_data)
		conn->dtls_data = dtls_openssl_data_create(conn,DTLSv1_server_method(),&bio_methods);

	struct dtls_openssl_data * d = (struct dtls_openssl_data*)conn->dtls_data;
	if (!d)
		return 0;

	SSL_set_psk_server_callback( d->ssl, psk_server_cb);

	int rc; 
//	do{ 
	int i;
	for (i=0; i<5; i++){	
		rc = SSL_accept(d->ssl);
		if (rc==0){
			int e;
			e = SSL_get_error(d->ssl,rc);
			switch (e){
				case SSL_ERROR_SYSCALL:
//					printf("syscall EOF!\n");
					break;
			}


//			printf ("UI! error?\n");

			char errstr[256];

			e = ERR_get_error();
			while (e!=0){
				ERR_error_string(e,errstr);
				cw_log(LOG_ERR,"SSL_accept - %s",errstr);
				e = ERR_get_error();
			}

			continue;
		}		
		if (rc == 1)
		{
			conn->read = dtls_openssl_read;
			conn->write = dtls_openssl_write;
			return 1;
		}


	}

	return 0;
	       
//	} while (rc == 0 );

	if (rc != 1){
		char errstr[256];
		int e = ERR_get_error();
		ERR_error_string(e,errstr);
		cw_log(LOG_ERR,"SSL_accept %s",errstr);
		return 0;
	}


	return 1;

/*	
	struct dtls_openssl_data * data = dtls_openssl_data_new();
	if (!data)
		return 0;

	SSL_CTX * ctx = dtls_openssl_create_ctx(DTLSv1_server_method());
	if (!ctx){
		free (data);
		return 0;
	}





//	dtls_init(conn,DTLSv1_server_method());

	struct dtls_data * dtls_data = conn->dtls_data;
	SSL * ssl = dtls_data->ssl;

//i	printf("bio connected\n");
//	BIO_ctrl(SSL_get_rbio(ssl), BIO_CTRL_DGRAM_SET_CONNECTED, 0, NULLi //&pinfo->client_addr.ss);
//	printf("bio connected done\n");
	int rc;
	printf("doing accepz\n");
//	do {
		rc = SSL_accept(ssl);
		printf("accept again\n");
//	} while(rc==0);		
	perror ("ERRP");

	printf("SSL accept rc %i\n",rc);
	int e = ERR_get_error();
	printf ("THE E: %i\n",e);


	char es[499];
	ERR_error_string(e,es);
	printf ("ES: %s\n",es);


	rc = SSL_get_error(ssl,rc);
	printf("ERROR RC: %08X\n",rc);

	exit(9);
*/
}


