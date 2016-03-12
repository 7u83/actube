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
 * @brief Implements DTLS BIO read/write functions.
 */


#include <arpa/inet.h>

#include "log.h"
#include "dbg.h"
#include "dtls.h"


/**
 * Reads CAPWAP DTLS data from a connection object.
 * @param conn conn object
 * @param out where to write data to
 * @param maxlen maximum number of bytes to read
 * @return the number of bytes read
 */ 
int dtls_bio_read(struct conn *conn, char *out, int maxlen)
{
	if (conn->dtls_buffer_len == 0) {
		int len = conn->recv_packet(conn, conn->dtls_buffer, 2048);
		if (len < 4)
			return 0;
		conn->dtls_buffer_len = len - 4;
		conn->dtls_buffer_pos = 4;
	}

	if (conn->dtls_buffer_len > maxlen) {
		memcpy(out, conn->dtls_buffer + conn->dtls_buffer_pos, maxlen);
		conn->dtls_buffer_len -= maxlen;
		conn->dtls_buffer_pos += maxlen;
		cw_dbg(DBG_DTLS_BIO, "SSL BIO read: (maxlen = %d), read %d, remain %d", maxlen,
		       maxlen, conn->dtls_buffer_len);
		cw_dbg_dmp(DBG_DTLS_BIO_DMP, (uint8_t *) out, maxlen, "Dump...");

		return maxlen;
	}

	memcpy(out, conn->dtls_buffer + conn->dtls_buffer_pos, conn->dtls_buffer_len);
	int ret = conn->dtls_buffer_len;
	conn->dtls_buffer_len = 0;
	cw_dbg(DBG_DTLS_BIO, "SSL BIO read: (maxlen = %d), read %d, remain %d", maxlen, ret,
	       conn->dtls_buffer_len);
	cw_dbg_dmp(DBG_DTLS_BIO_DMP, (uint8_t *) out, ret, "Dump...");
	return ret;
}

/**
 * Write DTLS data to a CAPWAP connection.
 * @param conn the connection
 * @param data data to write
 * @param len number of bytes to write
 * @return the number of bytes written
 */
int dtls_bio_write(struct conn *conn, const char *data, int len)
{
	uint8_t buffer[2048];
	*((uint32_t *) buffer) = htonl(1 << 24);
	memcpy(buffer + 4, data, len);
	int rc = conn->send_packet(conn, buffer, len + 4);
	if (rc>=0)
		rc-=4;
		

	cw_dbg(DBG_DTLS_BIO, "SSL BIO write: %d bytes, wrote=%d, ptr: %p", len, rc, data);
	cw_dbg_dmp(DBG_DTLS_BIO_DMP, (uint8_t *) data, len, "Dump ...");

	if (rc < 0)
		return rc;
	return rc ;
}
