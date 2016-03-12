

#include "gnutls/dtls.h"

#include "dtls.h"
#include "dtls_gnutls.h"

#include "conn.h"
#include "log.h"
#include "dbg.h"

#include "sock.h"

int dtls_gnutls_connect(struct conn *conn)
{
	struct dtls_gnutls_data *d;
	d = dtls_gnutls_data_create(conn,
				    GNUTLS_CLIENT | GNUTLS_DATAGRAM | GNUTLS_NONBLOCK);

	if (!d)
		return 0;

//      gnutls_dh_set_prime_bits(d->session, 512);
#if GNUTLS_VERSION_NUMBER >= 0x030100
	gnutls_handshake_set_timeout(d->session, GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);

	gnutls_dtls_set_data_mtu(d->session, 1500);
#endif
	gnutls_dtls_set_mtu(d->session, 1500);

	int rc;
	do {
		rc = gnutls_handshake(d->session);
	} while (rc == GNUTLS_E_AGAIN);


	if (rc < 0) {
		cw_log(LOG_ERR, "DTLS (gnutls) Can't connect to %s: %s",
		       sock_addr2str(&conn->addr), gnutls_strerror(rc));
		return 0;
	}


	cw_dbg(DBG_DTLS, "Handshake with %s successful", sock_addr2str(&conn->addr));

	conn->dtls_data = d;
	conn->read = dtls_gnutls_read;
	conn->write = dtls_gnutls_write;


	return 1;
}
