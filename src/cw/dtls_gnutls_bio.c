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


/**
 * @file
 * @brief DTLS functions specific to gnutls
 */

#include <stdio.h>
#include <errno.h>


#include <gnutls/gnutls.h>

#include "dtls.h"
#include "dtls_gnutls.h"
#include "capwap.h"
#include "timer.h"



ssize_t dtls_gnutls_bio_read(gnutls_transport_ptr_t b, void *out, size_t maxlen)
{
	struct conn *conn = (struct conn *)b;
	int rc = dtls_bio_read(conn,out,maxlen);
	if (rc<=0){
		errno = EAGAIN;
		return -1;
	}
	errno=0;
	return rc;
}

ssize_t dtls_gnutls_bio_write(gnutls_transport_ptr_t b, const void *data, size_t len)
{
	struct conn *conn = (struct conn*)b;
	return dtls_bio_write(conn,data,len);
}

int dtls_gnutls_bio_wait(gnutls_transport_ptr_t ptr, unsigned int ms)
{
	struct conn * conn = (struct conn*)ptr;
	time_t timer = cw_timer_start(ms/1000);
	int rc;

	uint8_t buffer[5];

	do {
		rc = conn->recv_packet_peek(conn,buffer,sizeof(buffer));

	}while(!cw_timer_timeout(timer) && rc==GNUTLS_E_AGAIN);

	return rc;

}


