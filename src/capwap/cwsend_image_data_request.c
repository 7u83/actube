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

#include "capwap.h"

void cwmsg_addelem_image_data(struct cwmsg *cwmsg, struct image_data *data)
{

}

int cwsend_image_data_request(struct conn * conn, struct image_data * data, struct image_identifier *id )
{
//	uint8_t buffer[CWMSG_MAX_SIZE];
	struct cwmsg * cwmsg = &conn->req_msg;
	cwmsg_init(cwmsg,conn->req_buffer,CWMSG_IMAGE_DATA_REQUEST,conn_get_next_seqnum(conn),0);

	if (data){
//		cwmsg_addelem(&cwmsg,CWMSGELEM_IMAGE_DATA,(uint8_t*)&data->type,sizeof(data->type));

		cwmsg_vaddelem(cwmsg,CWMSGELEM_IMAGE_DATA, 2,
			&data->type, sizeof(data->type),
			data->data, data->len
		);

	}
	return conn_send_cwmsg(conn,cwmsg);
}

