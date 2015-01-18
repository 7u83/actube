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
#include "conn.h"
#include "cwmsg.h"

int cwsend_discovery_request(struct conn * conn,struct radioinfo * radioinfo,struct wtpinfo * wtpinfo)
{
	uint8_t buffer[CWMSG_MAX_SIZE];
	struct cwmsg cwmsg;

	cwmsg_init(&cwmsg,buffer,CWMSG_DISCOVERY_REQUEST,conn_get_next_seqnum(conn),NULL /*radioinfo*/);
	
	cwmsg_addelem(&cwmsg,CWMSGELEM_DISCOVERY_TYPE,&wtpinfo->discovery_type,sizeof(uint8_t));
//	cwmsg_addelem_wtp_board_data(&cwmsg,wtpinfo);
//	cwmsg_addelem_wtp_descriptor(&cwmsg,wtpinfo);
	cwmsg_addelem(&cwmsg,CWMSGELEM_WTP_FRAME_TUNNEL_MODE,&wtpinfo->frame_tunnel_mode,sizeof(uint8_t));
	cwmsg_addelem(&cwmsg,CWMSGELEM_WTP_MAC_TYPE,&wtpinfo->mac_type,sizeof(uint8_t));

//cwmsg_addelem(&cwmsg,CWMSGELEM_CAPWAP_LOCAL_IPV4_ADDRESS);
//cwmsg_addelem_cw_local_ip_addr(&cwmsg,conn);

	cwmsg_addelem_wtp_radio_infos(&cwmsg,wtpinfo->radioinfo);

	if (conn->mtu_discovery)
		cwmsg_addelem_mtu_discovery_padding(&cwmsg,conn);




//uint8_t zven[] = {0xBF, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,0x66,0x69,0x73,0x68,0x00,0x02,0xFC,0xF5,0x28,0xCA,0xAE,0xE4,0x00,0x03,0x10,0x10,
//0x04, 0x10, 0x00,0x00,0x10,0x00,0x00,0x06,0xFC,0xF5,0x28,0xCA,0xAE,0xE5,0xC4,0x2E,0xC4,0x2E,0xC4,0x2E,0xC4,0x2E,0xC4,0x2E,
//0xC4,0x2E,0xC4,0x2E,0xC4,0x2E,0xC4,0x2E };


/*
uint8_t zven [] = {
      // 0x00, 00 03 7A 00 02 
	0x22, 0xE0, 00, 00, 00, 00, 00, 00, 00, 0x01, 0x66, 0x69, 0x73,0x68,0x00,0x02,0xFC,0xF5,0x28,0xCA,0xAE,0xE4,0x00,0x03,0x10,0x10 ,
        0x04,0x10,0x00, 00, 0x10, 00, 00,0x06,0xFC,0xF5,0x28,0xCA,0xAE,0xE5,0xAB,0x37,0xAB,0x37,0xAB,0x37,0xAB,0x37,0xAB,0x37,0xAB,0x37,0xAB,0x37,0xAB,0x37,0xAB,0x37,  
        00, 0x07, 00, 00, 0x27,0x11,0x00,0x08,0x00,0x00  };


*/


//	cwmsg_addelem_vendor_specific_payload(&cwmsg,890,2,zven,sizeof(zven));


	return conn_send_cwmsg(conn,&cwmsg);
}
