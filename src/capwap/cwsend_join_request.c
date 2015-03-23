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

#include <string.h>

#include "capwap.h"
#include "capwap_cisco.h"

#include "lwapp_cisco.h"

#include "conn.h"
#include "cwmsg.h"

#include "sock.h"

#include "acinfo.h" //Tube


int cwsend_join_request(struct conn *conn, struct radioinfo *radioinfo, struct wtpinfo *wtpinfo)
{
	uint8_t buffer[CWMSG_MAX_SIZE];
	struct cwmsg cwmsg;



	//radioinfo->rmac=0;

	cwmsg_init(&cwmsg, buffer, CWMSG_JOIN_REQUEST, conn_get_next_seqnum(conn), radioinfo);
	cwmsg.capwap_mode = conn->capwap_mode;

//	printf("Rad info rmac %s\n",sock_hwaddr2str(bstr_data(radioinfo->rmac),bstr_len(radioinfo->rmac)));

	/* Mandatory elements */

	/* location data */
	cwmsg_addelem(&cwmsg, CWMSGELEM_LOCATION_DATA, wtpinfo->location,
		      strlen((char *) wtpinfo->location));

	/* wtp board data */
	cwmsg_addelem_wtp_board_data(&cwmsg, wtpinfo);

	/* wtp descriptor */
	cwmsg_addelem_wtp_descriptor(&cwmsg, wtpinfo);

	/* wtp name */
	cwmsg_addelem(&cwmsg, CWMSGELEM_WTP_NAME, wtpinfo->name, strlen((char *) wtpinfo->name));

	/* session id */
	cwmsg_addelem_session_id(&cwmsg, wtpinfo->session_id);

	/* frame tunnel mode */
	cwmsg_addelem(&cwmsg, CWMSGELEM_WTP_FRAME_TUNNEL_MODE, &wtpinfo->frame_tunnel_mode,
		      sizeof(uint8_t));

	/* WTP MAC type */
	cwmsg_addelem(&cwmsg, CWMSGELEM_WTP_MAC_TYPE, &wtpinfo->mac_type, sizeof(uint8_t));


	/* local ip address */
	cwmsg_addelem_cw_local_ip_addr(&cwmsg, conn);

	/* maximum message length */
	uint16_t l = htons(wtpinfo->max_msg_len);
	cwmsg_addelem(&cwmsg, CWMSGELEM_MAXIMUM_MESSAGE_LENGTH, (uint8_t *) & l, sizeof(l));




	switch (conn->capwap_mode) {
		case CWMODE_CISCO:
			cwmsg_addelem_vendor_cisco_mwar_addr(&cwmsg,conn);

			uint8_t data207[4] = {1,1,0,1};

		        cwmsg_addelem_vendor_specific_payload(&cwmsg,CW_VENDOR_ID_CISCO,
                                        CW_CISCO_BOARD_DATA_OPTIONS,data207,4);



//		        cwmsg_ae(cw_addelem_vendor_specific_payload(&cwmsg,CW_VENDOR_ID_CISCO,
  //                                      CWVENDOR_CISCO_BOARD_DATA_OPTIONS,data207,4);

/*		        cwmsg_addelem_vendor_specific_payload(&cwmsg,CW_VENDOR_ID_CISCO,
                                        CWVENDOR_CISCO_AP_GROUP_NAME,(uint8_t*)"Tobias",strlen("Tobias"));
*/


			cwmsg_addelem_vendor_s_payload(&cwmsg, CW_VENDOR_ID_CISCO,
						CW_CISCO_AP_GROUP_NAME,(uint8_t *)"default-group",strlen("default-group"));


			break;
		default:
			/* ECN support */
			cwmsg_addelem(&cwmsg, CWMSGELEM_ECN_SUPPORT, &wtpinfo->ecn_support,
				      sizeof(uint8_t));
	}

	/* WTP radio information elements */
	cwmsg_addelem_wtp_radio_infos(&cwmsg, wtpinfo->radioinfo);


	/* Non-mandatory elements */

	switch (conn->capwap_mode) {
		case CWMODE_CISCO:
		{
			uint8_t mtu[2048];
			int l = lw_put_cisco_path_mtu(mtu,1485,1101);

			printf("Len = %d\n",l);

		//	cwmsg_addelem_vendor_specific_payload(&cwmsg,LW_VENDOR_CISCO,
		//			LW_ELEM_VENDOR_SPECIFIC,mtu,l);


		//	(&cwmsg)->pos+=4;	
			struct ac_info acinfo;
			extern struct ac_info wtp_acinfo;

//			memset(&acinfo,0,sizeof(struct ac_info));
//			cwmsg_addelem_ac_descriptor(&cwmsg,&wtp_acinfo,wtpinfo);
			
		}
			


	}


	printf("MTU %d\n",conn->mtu);
	return conn_send_cwmsg(conn, &cwmsg);
}
