#include "cw.h"

/**
 * Put an cw_ac_stauts structure to a buffer
 * @param dst destination buffer
 * @param s #cw_ac_status to put
 * @return number of bytes put
 * This function is only useful (used) in conjunction with 
 * putting AC Descriptor message elements.
 */
int cw_put_ac_status(uint8_t * dst, struct cw_ac_status *s, struct conn * conn)
{
	uint8_t *d = dst;

	d += cw_put_dword(d, (s->stations << 16) | (s->limit));
	d += cw_put_dword(d, (s->active_wtps << 16) | (s->max_wtps));


	int security = 0;
	security |= conn->dtls_cert_file ? CW_FLAG_AC_SECURITY_X : 0;
	security |= conn->dtls_psk ? CW_FLAG_AC_SECURITY_S : 0;


	d += cw_put_dword(d,
			  (security << 24) | (s->rmac_field << 16) | (s->dtls_policy));
	return d - dst;
}


