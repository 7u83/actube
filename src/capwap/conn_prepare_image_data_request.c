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
#include "lwapp.h"


#include "string.h" //tube
/*
static void cwmsg_addelem_image_data(struct cwmsg *cwmsg, struct image_data *data)
{

}
*/

int conn_prepare_image_data_request(struct conn * conn, struct image_data * data, struct image_identifier *id )
{
//	uint8_t buffer[CWMSG_MAX_SIZE];
	struct cwmsg * cwmsg = &conn->req_msg;
	cwmsg_init(cwmsg,conn->req_buffer,CWMSG_IMAGE_DATA_REQUEST,conn_get_next_seqnum(conn),0);

	if (data){
//		cwmsg_addelem(&cwmsg,CWMSGELEM_IMAGE_DATA,(uint8_t*)&data->type,sizeof(data->type));


		
//uint8_t buf[1024];
//memset(buf,0xff,1024);


		uint8_t type=3;
//		uint16_t checksum=htons(1968);

//uint8_t d[] = {0x01,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
		uint16_t checksum = htons( lw_checksum(data->data,data->len));
//		checksum = htons(1968);

		cwmsg_vaddelem(cwmsg,CWMSGELEM_IMAGE_DATA, 3,
			&type, 1,
			&checksum,2,
//			d, sizeof(d)
			data->data, data->len
//			buf,1024
		);


/*		cwmsg_addelem_image_identifier(cwmsg,890,(uint8_t*)"urf", 3);
		cwmsg_vaddelem(cwmsg,CWMSGELEM_IMAGE_DATA, 2,
			&data->type, sizeof(data->type),
			data->data, data->len
		);
*/

	}
//	return conn_send_cwmsg(conn,cwmsg);
	return 0;
}

