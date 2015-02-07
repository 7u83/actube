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

#include <stdlib.h>

#include <gnutls/gnutls.h>


#include "dtls_gnutls.h"

#include "conn.h"
#include "cw_log.h"


struct dtls_gnutls_data{
	gnutls_session_t session;
	gnutls_certificate_credentials_t x509_cred;
	gnutls_priority_t priority_cache;
};

void dtls_gnutls_data_destroy(struct dtls_gnutls_data *d)
{
	free(d);
}

struct dtls_gnutls_data * dtls_gnutls_data_create(struct conn * conn)
{
	struct dtls_gnutls_data * d = malloc(sizeof(struct dtls_gnutls_data));
	if (!d)
		return 0;



	
	gnutls_certificate_allocate_credentials(&d->x509_cred);


  	int rc;

	/* Set credentials */
	rc  = gnutls_certificate_set_x509_key_file(d->x509_cred, conn->dtls_cert_file,
                                                 conn->dtls_key_file,
                                                 GNUTLS_X509_FMT_PEM);

	if (rc<0){
		cw_log(LOG_ERR,"DTLS - Can't set cert/key: %s",gnutls_strerror(rc));
		dtls_gnutls_data_destroy(d);
		return 0;
	}
	
	/* Set ciphers */
	const char * errpos;
	rc = 	gnutls_priority_init(&d->priority_cache,
                             conn->dtls_cipher,
                             &errpos);
	if (rc<0){
		cw_log(LOG_ERR,"DTLS - Can't init ciphers '%s' at '%s' : %s",conn->dtls_cipher,errpos,gnutls_strerror(rc));
		dtls_gnutls_data_destroy(d);
		return 0;
	}


	rc = gnutls_init(&d->session, GNUTLS_SERVER );
	if (rc<0){
		cw_log(LOG_ERR,"DTLS - Can't init session: %s",gnutls_strerror(rc));
		dtls_gnutls_data_destroy(d);
		return 0;
	}

	gnutls_transport_set_ptr(d->session,conn);


	rc = gnutls_priority_set(d->session, d->priority_cache);
	if (rc<0){
		cw_log(LOG_ERR,"DTLS - Can't set priority: %s",gnutls_strerror(rc));
		dtls_gnutls_data_destroy(d);
		return 0;
	}


	rc = gnutls_credentials_set(d->session, GNUTLS_CRD_CERTIFICATE,
                                       d->x509_cred);
	
	if (rc<0){
		cw_log(LOG_ERR,"DTLS - Can't set credentials: %s",gnutls_strerror(rc));
		dtls_gnutls_data_destroy(d);
		return 0;
	}

	gnutls_transport_set_pull_function(d->session,dtls_gnutls_bio_read);
	gnutls_transport_set_push_function(d->session,dtls_gnutls_bio_write);



	return d;	
}



int dtls_gnutls_accept(struct conn * conn)
{
	struct dtls_gnutls_data *d;
	d = dtls_gnutls_data_create(conn);
	if (!d)
		return 0;


	int rc;
	
	do {
		rc = gnutls_handshake(d->session);
	}while(rc == GNUTLS_E_AGAIN);

	cw_log(LOG_ERR,"DTLS - Can't set credentials: %s",gnutls_strerror(rc));

   //    gnutls_certificate_allocate_credentials(&x509_cred);


return 0;

	return 1;	
}


const char * dtls_gnutls_get_cipher(struct conn * conn)
{
	return "Unknown";
}
