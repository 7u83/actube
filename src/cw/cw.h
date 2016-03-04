#ifndef __CW_H
#define __CW_H

#include "mbag.h"
#include "conn.h"
#include "action.h"

#include "lw.h"

/**
 * @defgroup CW CW
 * @{
 */

extern int cw_read_wtp_descriptor(mbag_t mbag, struct conn *conn,
			   struct cw_action_in *a, uint8_t * data, int len);

extern int cw_read_wtp_descriptor_7(mbag_t mbag, struct conn *conn,
				    struct cw_action_in *a, uint8_t * data, int len);

extern int cw_read_wtp_descriptor_versions(mbag_t mbag, uint8_t * data,
					   int len);

/* Use some macros from LWAPP */

/**
 * Put a byte to an output buffer. 
 * see #lw_put_byte 
 */
#define cw_put_byte lw_put_byte
/**
 * Put a word to an output buffee.
 * see #lw_put_word
 */
#define cw_put_word lw_put_word
#define cw_put_dword lw_put_dword
#define cw_put_data lw_put_data
#define cw_put_bstr lw_put_bstr
#define cw_put_bstr16 lw_put_bstr16


/**
 *@}
 */

#endif
