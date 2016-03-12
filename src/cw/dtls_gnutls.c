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
#include <errno.h>

#include <gnutls/gnutls.h>
#include <gnutls/dtls.h>

#include "dtls_gnutls.h"

#include "conn.h"
#include "log.h"
#include "dbg.h"
#include "cw_util.h"
#include "timer.h"

int dtls_gnutls_init()
{
	cw_dbg(DBG_INFO,"Init SSL library - using GnuTLS %s",gnutls_check_version(NULL));
	gnutls_global_init();
	return 1;
}

int dtls_gnutls_shutdown(struct conn *conn)
{
	/* implement it */
	return 1;
}

void dtls_gnutls_data_destroy(struct dtls_gnutls_data *d)
{
	free(d);
}

int dtls_gnutls_write(struct conn * conn, const uint8_t *buffer, int len)
{
	struct dtls_gnutls_data * d = conn->dtls_data;

	int rc = gnutls_record_send(d->session,buffer,len);

	if ( rc < 0 ){
		cw_log(LOG_ERR, "DTLS - write error: %s", gnutls_strerror(rc));
		conn->dtls_error=1;
		errno=ECONNRESET;
		return -1;
	}

	return rc;
}



int dtls_gnutls_read(struct conn * conn, uint8_t *buffer, int len)
{
	uint8_t seq[8];
	
	struct dtls_gnutls_data * d = conn->dtls_data;
	int rc = gnutls_record_recv_seq(d->session,buffer,len,seq);

	if (rc==0) {
		errno = ECONNRESET;
		return -1;
	}

	if ( rc == GNUTLS_E_AGAIN ){
		errno = EAGAIN;
		return -1;
	}

	if ( rc < 0 ){
		cw_log(LOG_ERR, "DTLS - read error: %s", gnutls_strerror(rc));
		conn->dtls_error=1;
		errno=ECONNRESET;
		return -1;
	}

	return rc;
}


static int verify_cert(gnutls_session_t sess)
{
	cw_dbg(DBG_DTLS,"Verify cert");
	return 0;
}


static void dtls_log_cb(int level, const char * str)
{
	if (!cw_dbg_is_level(DBG_DTLS_DETAIL))
		return;

	switch (level){
		case 2:
		case 6:
		case 4:
			return;


	}


	
	
	char buf[2048];
	strcpy(buf,str);
	char *c = strchr(buf,'\n');
	*c=0;
	cw_dbg(DBG_DTLS_DETAIL,"%s",buf);
}

struct dtls_gnutls_data *dtls_gnutls_data_create(struct conn *conn,int config)
{
	struct dtls_gnutls_data *d = malloc(sizeof(struct dtls_gnutls_data));
	if (!d)
		return 0;

	gnutls_global_set_log_level(10);
	gnutls_global_set_log_function(dtls_log_cb);

	gnutls_certificate_allocate_credentials(&d->x509_cred);


	int rc;

	/* Set credentials */

	if (conn->dtls_cert_file && conn->dtls_key_file){

		rc = gnutls_certificate_set_x509_key_file(d->x509_cred, conn->dtls_cert_file,
							  conn->dtls_key_file, GNUTLS_X509_FMT_PEM);

		if (rc < 0) {
			cw_log(LOG_ERR, "DTLS - Can't set cert/key: %s", gnutls_strerror(rc));
			dtls_gnutls_data_destroy(d);
			return 0;
		}
	}



	int bits;
#if GNUTLS_VERSION_NUMBER >= 0x030100
	bits = gnutls_sec_param_to_pk_bits(GNUTLS_PK_DH, GNUTLS_SEC_PARAM_INSECURE);
#else
	bits = gnutls_sec_param_to_pk_bits(GNUTLS_PK_DH, GNUTLS_SEC_PARAM_WEAK);
#endif
        /* Generate Diffie-Hellman parameters - for use with DHE
         * kx algorithms. When short bit length is used, it might
         * be wise to regenerate parameters often.
         */
        gnutls_dh_params_init(&d->dh_params);
        gnutls_dh_params_generate2(d->dh_params, bits);
        gnutls_certificate_set_dh_params(d->x509_cred, d->dh_params);




	/* Set ciphers */
	const char *errpos;
	rc = gnutls_priority_init(&d->priority_cache, conn->dtls_cipher, &errpos);
	if (rc < 0) {
		cw_log(LOG_ERR, "DTLS - Can't init ciphers '%s' at '%s' : %s", conn->dtls_cipher,
		       errpos, gnutls_strerror(rc));
		dtls_gnutls_data_destroy(d);
		return 0;
	}


	rc = gnutls_init(&d->session, config); 
	if (rc < 0) {
		cw_log(LOG_ERR, "DTLS - Can't init session: %s", gnutls_strerror(rc));
		dtls_gnutls_data_destroy(d);
		return 0;
	}

	gnutls_transport_set_ptr(d->session, conn);


	rc = gnutls_priority_set(d->session, d->priority_cache);
	if (rc < 0) {
		cw_log(LOG_ERR, "DTLS - Can't set priority: %s", gnutls_strerror(rc));
		dtls_gnutls_data_destroy(d);
		return 0;
	}


	rc = gnutls_credentials_set(d->session, GNUTLS_CRD_CERTIFICATE, d->x509_cred);

	if (rc < 0) {
		cw_log(LOG_ERR, "DTLS - Can't set credentials: %s", gnutls_strerror(rc));
		dtls_gnutls_data_destroy(d);
		return 0;
	}

	gnutls_certificate_set_verify_function(d->x509_cred,verify_cert);


	gnutls_transport_set_pull_function(d->session, dtls_gnutls_bio_read);
	gnutls_transport_set_push_function(d->session, dtls_gnutls_bio_write);
	gnutls_transport_set_pull_timeout_function(d->session, dtls_gnutls_bio_wait);


#if GNUTLS_VERSION_NUMBER >= 0x030100
	gnutls_handshake_set_timeout(d->session, GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);

	gnutls_dtls_set_data_mtu(d->session, conn->dtls_mtu);
#endif
	gnutls_dtls_set_mtu(d->session, conn->dtls_mtu);




	return d;
}


