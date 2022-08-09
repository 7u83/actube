/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
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

/**
 * @file
 * @brief Functions for OpenSSL BIO
 */


#include <errno.h>
#include <arpa/inet.h>
#include "dtls_openssl.h"
#include "dtls_common.h"


#include <openssl/bio.h>

#include "conn.h"
#include "log.h"
#include "dbg.h"


BIO_METHOD *dtls_openssl_bio_method()
{
	int index = BIO_get_new_index() ;
/*	index = BIO_TYPE_DGRAM;*/
	cw_dbg(DBG_DTLS_BIO, "Creating new OpenSSL BIO Methods");

	BIO_METHOD * bio_methods;
	bio_methods = BIO_meth_new(index,"CW Packet");
	if (!bio_methods){
		return NULL;
	}

	BIO_meth_set_write(bio_methods,dtls_openssl_bio_write);
	BIO_meth_set_read(bio_methods,dtls_openssl_bio_read);
	BIO_meth_set_puts(bio_methods,dtls_openssl_bio_puts);
	BIO_meth_set_ctrl(bio_methods,dtls_openssl_bio_ctrl);
	BIO_meth_set_create(bio_methods,dtls_openssl_bio_new);
	BIO_meth_set_destroy(bio_methods,dtls_openssl_bio_free);


/*	bio_methods = BIO_mth_new(BIO_TYPE_DGRAM,"CW Packet");*/


/*	bio_methods.type = BIO_TYPE_DGRAM;
	bio_methods.name = "CW packet";
	bio_methods.bwrite = dtls_openssl_bio_write;
	bio_methods.bread = dtls_openssl_bio_read;
	bio_methods.bputs = dtls_openssl_bio_puts;
	bio_methods.bgets = 0;
	bio_methods.ctrl = dtls_openssl_bio_ctrl;
	bio_methods.create = dtls_openssl_bio_new;
	bio_methods.destroy = dtls_openssl_bio_free;
	bio_methods.callback_ctrl = 0;
*/

	return bio_methods;
}

int dtls_openssl_bio_write(BIO * b, const char *data, int len)
{
/*	struct cw_Conn *conn = b->ptr;*/
	struct cw_Conn *conn = BIO_get_data(b);
	return dtls_bio_write(conn, data, len);
}


int dtls_openssl_bio_read(BIO * b, char *out, int maxlen)
{
	/*struct cw_Conn *conn = b->ptr;*/
	struct cw_Conn *conn = BIO_get_data(b);
	return dtls_bio_read(conn, out, maxlen);
}



int dtls_openssl_bio_new(BIO * bi)
{
	/*
	bi->init = 1;
	bi->num = 0;
	bi->flags = 0;
	bi->ptr = NULL;
*/
	BIO_set_init(bi,1);

	cw_dbg(DBG_DTLS_BIO, "Creating new OpenSSL BIO");
	return 1;
}



int dtls_openssl_bio_puts(BIO * b, const char *str)
{
	cw_dbg(DBG_DTLS_BIO, "SSL BIO puts: '%s'", str);
	return dtls_openssl_bio_write(b, str, strlen(str));
}

int dtls_openssl_bio_free(BIO * bio)
{
	if (bio == NULL)
		return 0;
	return 1;
}



long dtls_openssl_bio_ctrl(BIO * b, int cmd, long num, void *ptr)
{
	/*struct cw_Conn *conn = b->ptr;*/
	struct cw_Conn *conn = BIO_get_data(b);

	long ret = 1;
	switch (cmd) {
		case BIO_CTRL_DUP:
			ret = 1;
			break;

		case BIO_CTRL_FLUSH:
			ret = 1;
			break;

		case BIO_CTRL_DGRAM_QUERY_MTU:
		case BIO_CTRL_DGRAM_GET_MTU:
			ret = conn->dtls_mtu;
			break;

		case BIO_CTRL_DGRAM_SET_MTU:
			ret = 1;
			break;

		default:
			ret = 0;
			break;
	}

	return ret;
}
