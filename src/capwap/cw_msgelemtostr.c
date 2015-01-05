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
#include "capwap_ieee80211.h"


const char * cw_msgelemtostr(int elem)
{
	switch (elem){
		case CWMSGELEM_AC_DESCRIPTOR:
			return "ac descriptor";
		
/*   AC IPv4 List                                          2
   AC IPv6 List                                          3
*/   
		case CWMSGELEM_AC_NAME:
			return "ac name";
/*
   AC Name with Priority                                 5
   AC Timestamp                                          6
   Add MAC ACL Entry                                     7
   Add Station                                           8
   Reserved                                              9
*/   
		case CWMSGELEM_CONTROL_IPV4_ADDRESS:
			return "ctrl ipv4 addr";
		case CWMSGELEM_CONTROL_IPV6_ADDRESS:
			return "ctrl ipv6 addr";
   
		case CWMSGELEM_CAPWAP_LOCAL_IPV4_ADDRESS:
			return "capwap local ipv4 addr";
		case CWMSGELEM_CAPWAP_LOCAL_IPV6_ADDRESS:
			return "capwap local ipv6 addr";
/*   CAPWAP Timers                                        12
   CAPWAP Transport Protocol                            51
   Data Transfer Data                                   13
   Data Transfer Mode                                   14
   Decryption Error Report                              15
   Decryption Error Report Period                       16
   Delete MAC ACL Entry                                 17
   Delete Station                                       18
   Reserved                                             19
*/   
		case CWMSGELEM_DISCOVERY_TYPE:
			return "discovery type";
/*   
   Duplicate IPv4 Address                               21
   Duplicate IPv6 Address                               22
*/   
		case CWMSGELEM_ECN_SUPPORT:
			return "ecn support";
 /*  Idle Timeout                                         23
*/
		case CWMSGELEM_IMAGE_DATA:
			return "image data";

		case CWMSGELEM_IMAGE_IDENTIFIER:
			return "image identifier";


  /* Image Information                                    26
   Initiate Download                                    27
   */
		case CWMSGELEM_LOCATION_DATA:
			return "location data";

		case CWMSGELEM_MAXIMUM_MESSAGE_LENGTH:
			return "max msg len";
		case CWMSGELEM_MTU_DISCOVERY_PADDING:
			return "mtu discovery padding";

		case CWMSGELEM_RADIO_ADMINISTRATIVE_STATE:
			return "radio administrative state";

		case CWMSGELEM_RADIO_OPERATIONAL_STATE:
			return "radio operational state";

/*   Radio Operational State                              32
*/
		case CWMSGELEM_RESULT_CODE:
			return "result code";

/*   Returned Message Element                             34
*/   
		case CWMSGELEM_SESSION_ID:
			return "session id";

		case CWMSGELEM_STATISTICS_TIMER:
			return "statistics timer";

		case CWMSGELEM_VENDOR_SPECIFIC_PAYLOAD:
			return "vendor specific payload";

 
		case CWMSGELEM_WTP_BOARD_DATA:
			return "wtp board data";
   
		case CWMSGELEM_WTP_DESCRIPTOR:
			return "wtp descriptor";
/*   WTP Fallback                                         40
  */
		case CWMSGELEM_WTP_FRAME_TUNNEL_MODE:
			return "frame tunnel mode";

		case CWMSGELEM_RESERVED_1:
			return "reserved (42)";
/*
   Reserved                                             43
*/   
		case CWMSGELEM_WTP_MAC_TYPE:
			return "mac type";

		case CWMSGELEM_WTP_NAME:
			return "wtp name";
/*   
   Unused/Reserved                                      46
   WTP Radio Statistics                                 47
*/


		case CWMSGELEM_WTP_REBOOT_STATISTICS:
			return "wtp reboot statistics";
 
/*   WTP Static IP Address Information                    49
*/




		case CWMSGELEM_IEEE80211_WTP_RADIO_INFO:
			return "802.11: wtp radio info";



		case XCWMSGELEM_CAPWAP_LOCAL_IP_ADDRESS:
			return "local ip v4/v6 address";

		case XCWMSGELEM_CAPWAP_RADIO_INFO:
			return "radio info";


	}

	return "unknown";

}
