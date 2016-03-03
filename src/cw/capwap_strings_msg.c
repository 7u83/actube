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
 * @brief Definition of CAPWAP Message Strings
 */ 

#include "capwap.h"

/**
 * CAPWAP Message Strings
 */ 
struct cw_strlist_elem capwap_strings_msg[] = {
{ CW_MSG_DISCOVERY_REQUEST, "Discovery Request" },
{ CW_MSG_DISCOVERY_RESPONSE,"Discovery Response" },
{ CW_MSG_JOIN_REQUEST, "Join Request"},
{ CW_MSG_JOIN_RESPONSE,"Join Response"},
{ CW_MSG_CONFIGURATION_STATUS_REQUEST,"Configuration Status Request"},
{ CW_MSG_CONFIGURATION_STATUS_RESPONSE,"Configuration Status Response"},
{ CW_MSG_CONFIGURATION_UPDATE_REQUEST,"Configuration Update Request"},
{ CW_MSG_CONFIGURATION_UPDATE_RESPONSE,"Configuration Update Response"},
{ CW_MSG_WTP_EVENT_REQUEST,"WTP Event Request"},
{ CW_MSG_WTP_EVENT_RESPONSE,"WTP Event Response"},
{ CW_MSG_CHANGE_STATE_EVENT_REQUEST,"Change State Event Request"},
{ CW_MSG_CHANGE_STATE_EVENT_RESPONSE,"Change State Event Response"},
{ CW_MSG_ECHO_REQUEST,"Echo Request"},
{ CW_MSG_ECHO_RESPONSE,"Echo Response"},
{ CW_MSG_IMAGE_DATA_REQUEST,"Image Data Request"},
{ CW_MSG_IMAGE_DATA_RESPONSE,"Image Data Response"},
{ CW_MSG_RESET_REQUEST,"Reset Request"},
{ CW_MSG_RESET_RESPONSE,"Reset Response"},
{ CW_MSG_PRIMARY_DISCOVERY_REQUEST,"Primary Discovery Request"},
{ CW_MSG_PRIMARY_DISCOVERY_RESPONSE,"Primary Discovery Response"},

/*           Data Transfer Request               21
           Data Transfer Response              22
*/
{CW_MSG_CLEAR_CONFIGURATION_REQUEST,"Clear Configuration Request"}, 
{CW_MSG_CLEAR_CONFIGURATION_RESPONSE,"Clear Configuration Response"},
/*           Station Configuration Request       25
           Station Configuration Response      26
*/

{CW_STR_STOP,"Unknown Message Type"}

};


