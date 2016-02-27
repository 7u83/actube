#ifndef __CW_H
#define __CW_H

#include "mbag.h"
#include "conn.h"
#include "action.h"

extern int cw_read_wtp_descriptor_7(mbag_t mbag, struct conn *conn,
				    struct cw_action_in *a, uint8_t * data, int len);

extern int cw_read_wtp_descriptor_versions(mbag_t mbag, uint8_t * data,
					   int len);


#endif
