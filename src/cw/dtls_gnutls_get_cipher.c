

#include "dtls_common.h"
#include "dtls_gnutls.h"

const char *dtls_gnutls_get_cipher(struct conn *conn, char *dst)
{
	struct dtls_gnutls_data * d;
	const char *comp, *cipher, *mac, *proto, *kxname, *auth;
	gnutls_kx_algorithm_t kx;
	gnutls_credentials_type_t cred;
	
	if (!conn->dtls_data){
		sprintf(dst, "%s","None");
		return dst;
	}

	d = (struct dtls_gnutls_data*)conn->dtls_data;
	if ( !d->session ){
		sprintf(dst, "%s","None");
		return dst;
	}
	kx = gnutls_kx_get(d->session);
	kxname = gnutls_kx_get_name(kx);

	cred = gnutls_auth_get_type(d->session);

	proto = gnutls_protocol_get_name(gnutls_protocol_get_version(d->session));
	comp = gnutls_compression_get_name(gnutls_compression_get(d->session));
	cipher = gnutls_cipher_get_name(gnutls_cipher_get(d->session));
	mac = gnutls_mac_get_name(gnutls_mac_get(d->session));
	sprintf(dst,"cipher: %s/%s/%s/%s/%s",proto,kxname,cipher,mac,comp);

	return dst;
}
