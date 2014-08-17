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

const char * cw_msgtostr(int type)
{
	switch(type){
		case CWMSG_DISCOVERY_REQUEST:
			return "discovery request";

		case CWMSG_DISCOVERY_RESPONSE:
			return "discovery response";

		case CWMSG_JOIN_REQUEST:
			return "join request";

		case CWMSG_JOIN_RESPONSE:
			return "join response";

		case CWMSG_CONFIGURATION_STATUS_REQUEST:
			return "configuration status request";

		case CWMSG_CONFIGURATION_STATUS_RESPONSE:
			return "configuration status response";

/*           Configuration Update Request         7
           Configuration Update Response        8
           WTP Event Request                    9
           WTP Event Response                  10
*/

		case CWMSG_CHANGE_STATE_EVENT_REQUEST:
			return "change state event request";
		case CWMSG_CHANGE_STATE_EVENT_RESPONSE:
			return "change state event response";

/*
           Change State Event Request          11
           Change State Event Response         12
*/	   
		case CWMSG_ECHO_REQUEST:
			return "echo request";
		case CWMSG_ECHO_RESPONSE:
			return "echo response";

		case CWMSG_IMAGE_DATA_REQUEST:
			return "image data request";
		case CWMSG_IMAGE_DATA_RESPONSE:
			return "image data response";

/*          Image Data Request                  15
           Image Data Response                 16
*/

		case CWMSG_RESET_REQUEST:
			return "reset request";
		case CWMSG_RESET_RESPONSE:
			return "reset response";

/*
           Reset Request                       17
           Reset Response                      18
*/	   
		case CWMSG_PRIMARY_DISCOVERY_REQUEST:
			return "primary discovery request";
		case CWMSG_PRIMARY_DISCOVERY_RESPONSE:
			return "primary discovery response";

/*           Data Transfer Request               21
           Data Transfer Response              22
           Clear Configuration Request         23
           Clear Configuration Response        24
           Station Configuration Request       25
           Station Configuration Response      26
*/

	
	}

	return "unknown";
}

