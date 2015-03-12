
#include "dtls.h"
#include "dtls_gnutls.h"

#include "conn.h"
#include "cw_log.h"

int dtls_gnutls_connect(struct conn *conn)
{
	struct dtls_gnutls_data * d;
	d = dtls_gnutls_data_create(conn,GNUTLS_CLIENT | GNUTLS_DATAGRAM | GNUTLS_NONBLOCK);

//	gnutls_dh_set_prime_bits(d->session, 512);
	gnutls_handshake_set_timeout(d->session,GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);	

	int rc;	
	do {
		rc = gnutls_handshake(d->session);
	}while (rc==GNUTLS_E_AGAIN);

	
	if (rc < 0){
		cw_log(LOG_ERR,"Can't connect: %s",gnutls_strerror(rc));
		return 0;
	}


	cw_dbg(DBG_DTLS,"DTLS - Handshake successful");

	conn->dtls_data=d;
	conn->read = dtls_gnutls_read;
	conn->write = dtls_gnutls_write;

	
	return 1;
}

