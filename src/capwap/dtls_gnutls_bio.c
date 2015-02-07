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


#include <gnutls/gnutls.h>

#include "dtls.h"
#include "dtls_gnutls.h"

ssize_t dtls_gnutls_bio_read(gnutls_transport_ptr_t b, void *out, size_t maxlen)
{

	struct conn *conn = (struct conn *)b;
	return dtls_bio_read(conn,out,maxlen);
}

ssize_t dtls_gnutls_bio_write(gnutls_transport_ptr_t b, const void *data, size_t len)
{

	struct conn *conn = (struct conn*)b;
	return dtls_bio_write(conn,data,len);
}


