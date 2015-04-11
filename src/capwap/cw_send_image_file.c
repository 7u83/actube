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
 * @breif send image file
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>


#include "capwap.h"
#include "lwapp.h"

#include "sock.h"
#include "cw_log.h"


/**
 * Send an image file to an AP
 * 
 */ 
void cw_send_image_file(struct conn *conn)
{

/*
	FILE *infile;
	infile = fopen(filename, "rb");
	if (!infile) {
		cw_log(LOG_ERR, "Can't open image file %s:%s", filename, strerror(errno));
		return;
	}


	cw_log(LOG_INFO, "Sending image file %s to %s", filename, sock_addr2str(&conn->addr));
*/

	

	


	uint8_t buffer[1024];	/* buffer MUST be 1024 */


	struct cwimage_data data;
	memset(&data,0,sizeof(struct cwimage_data));
	data.data = buffer;


/*
	conn->request_handler = cw_handle_echo_request;
	conn->request_handler_param = conn;
*/

	int bl = 0;

	do {

		data.len = fread(buffer, 1, sizeof(buffer), infile);

		if (feof(infile))
			data.type = 2;
		else
			data.type = 1;

		cw_dbg(DBG_CW_IMG_DTL, "Sending img data request, block=%d, len=%d, ch=%d\n", bl,
		       data.len, lw_checksum(data.data, data.len));


		bl++;

	//	conn_prepare_image_data_request(conn, &data, 0);
	//	cwrmsg = conn_send_request(conn);

		if (!cwrmsg) {
			cw_log(LOG_ERR,"Error sneding image file to %s",sock_addr2str(&conn->addr));
			break;
		}


	} while (!feof(infile));

}
