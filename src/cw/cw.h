#ifndef __CW_H
#define __CW_H

#include <arpa/inet.h>

#include "bstr.h"
#include "mbag.h"
#include "conn.h"
#include "action.h"

#include "lw.h"

/**
 * @defgroup CW CW
 * @{
 */


/* Use some macros from LWAPP */

/**
 * Put a byte to an output buffer. 
 * see #lw_put_byte 
 */
#define cw_put_byte lw_put_byte

/**
 * Put a word to an output buffer.
 * see #lw_put_word
 */
#define cw_put_word lw_put_word

/**
 * Put a dword 
 * see #lw_put_dword
 */
#define cw_put_dword lw_put_dword

/**
 * Put some bytes
 * See #lw_put_data
 */
#define cw_put_data lw_put_data

/**
 * Put a bstr_t object
 * see #lw_put_bstr
 */
#define cw_put_bstr lw_put_bstr

/**
 * Put bstr16_t object
 * see #lw_put_bstr16
 */
#define cw_put_bstr16 lw_put_bstr16

/**
 * Set dword
 * see #lw_set_dword
 */
#define cw_set_dword lw_set_dword

#define cw_get_byte lw_get_byte
#define cw_get_word lw_get_word
#define cw_get_dword lw_get_dword



static inline int cw_put_version(uint8_t * dst, uint16_t subelem_id, bstrv_t v)
{
	uint8_t *d = dst;
	d += cw_put_dword(d, bstrv_get_vendor_id(v));
	d += cw_put_dword(d, (subelem_id << 16) | bstrv_len(v));
	d += cw_put_data(d, bstrv_data(v), bstrv_len(v));
	return d - dst;
}


extern int cw_read_wtp_descriptor(mbag_t mbag, struct conn *conn,
				  struct cw_action_in *a, uint8_t * data, int len);

extern int cw_read_wtp_descriptor_7(mbag_t mbag, struct conn *conn,
				    struct cw_action_in *a, uint8_t * data, int len);

extern int cw_read_wtp_descriptor_versions(mbag_t mbag, uint8_t * data, int len);


extern int cw_in_check_generic_resp(struct conn *conn, struct cw_action_in *a,
				    uint8_t * data, int len, struct sockaddr *from);


/**
 * @defgroup CWELEMIN Input Handlers for Message Elements
 * @{
 */
extern int cw_in_vendor_specific_payload(struct conn *conn, struct cw_action_in *a,
					 uint8_t * data, int len, struct sockaddr *from);

extern int cw_in_capwap_control_ip_address(struct conn *conn, struct cw_action_in *a,
					   uint8_t * data, int len,
					   struct sockaddr *from);
/**
 * @}
 */


/**
 * @defgroup CWELEMOUT Output Handlers for Message Elements
 * @{
 */
extern int cw_out_capwap_local_ip_address(struct conn *conn, struct cw_action_out *action,
					  uint8_t * dst);
/**
 * @}
 */


/**
 *@}
 */

#endif
