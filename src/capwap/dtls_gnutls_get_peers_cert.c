#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "conn.h"
#include "dtls.h"
#include "dtls_gnutls.h"

struct dtls_ssl_cert dtls_gnutls_get_peers_cert(struct conn * conn,unsigned int n)
{
	struct dtls_gnutls_data *d;
	d=(struct dtls_gnutls_data*)conn->dtls_data;

	const gnutls_datum_t * clist;

	struct dtls_ssl_cert cert;

	unsigned int len;
	clist = gnutls_certificate_get_peers(d->session,&len);

	cert.size = clist[n].size;
	cert.data= clist[n].data;
	return cert;
}


