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


#include "dtls_openssl.h"

const char * dtls_openssl_get_cipher(struct conn * conn, char *dst)
{
	struct dtls_openssl_data * d;
	const SSL_CIPHER * c;
	if (!conn->dtls_data){
		sprintf(dst, "%s","None");
		return dst;
	}

	d = (struct dtls_openssl_data*)conn->dtls_data;

	if ( !d->ssl ){
		sprintf(dst, "%s","None");
		return dst;
	}

	c = SSL_get_current_cipher(d->ssl);
	sprintf(dst,"%s",SSL_CIPHER_get_name(c));
	return dst;
}
