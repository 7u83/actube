#include "capwap.h"

struct cw_str capwap_strings_msg[] = {
{ CW_MSG_DISCOVERY_REQUEST, "Discovery Request" },
{ CW_MSG_DISCOVERY_RESPONSE,"Discovery Response" },
{ CW_MSG_JOIN_REQUEST, "Join Request"},
{ CW_MSG_JOIN_RESPONSE,"Join Response"},
{ CW_MSG_CONFIGURATION_STATUS_REQUEST,"Configuration Request"},
{ CW_MSG_CONFIGURATION_STATUS_RESPONSE,"Configuration Response"},
{ CW_MSG_CONFIGURATION_UPDATE_REQUEST,"Update Request"},
{ CW_MSG_CONFIGURATION_UPDATE_RESPONSE,"Update Response"},
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
           Clear Configuration Request         23
           Clear Configuration Response        24
           Station Configuration Request       25
           Station Configuration Response      26
*/

{CW_STR_STOP,"Unknown Message Type"}

};


