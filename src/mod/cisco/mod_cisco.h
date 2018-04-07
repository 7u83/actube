#ifndef __MOD_CISCO_H
#define __MOD_CISCO_H

#include <stdint.h>
#include "cw/msgset.h"

int cisco_in_wtp_descriptor(struct cw_ElemHandler *eh, struct cw_ElemHandlerParams * params, uint8_t * data,
                         int len);

int cisco_out_wtp_descriptor(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
                        , uint8_t * dst);

int cisco_in_ac_descriptor(struct cw_ElemHandler *eh, 
			struct cw_ElemHandlerParams *params, 
			uint8_t * data, int len);
			
int cisco_out_ac_descriptor(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst);

int cisco_out_radio_generic(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params
			, uint8_t * dst);

int cisco_out_ap_timesync(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst);
		
int cisco_in_spam_vendor_specific(struct cw_ElemHandler *eh, 
			struct cw_ElemHandlerParams *params, 
			uint8_t * data, int len);

int cisco_in_lw_path_mtu(struct cw_ElemHandler *eh, 
			struct cw_ElemHandlerParams *params, 
			uint8_t * data, int len);

int cisco_out_capwap_local_ip_address(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst);

int cisco_out_lw_path_mtu(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst);
		
struct cw_MsgSet * cisco_register_msg_set(struct cw_MsgSet * set, int mode);



#endif
