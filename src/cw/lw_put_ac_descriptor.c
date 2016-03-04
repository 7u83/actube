#include <string.h>

#include "lwapp.h"
#include "lw.h"

int lw_put_ac_descriptor(uint8_t * dst)
{
	uint8_t *d=dst;
	d+=lw_put_byte(d,0); /* Cisco MWAR type */

//	d+=lw_put_bstr(d,acinfo->hardware_version); /* length must be 4 */
//	d+=lw_put_bstr(d,acinfo->software_version); /* length must be 4 */
	d+=lw_put_word(d,200);
	d+=lw_put_word(d,1000);
	d+=lw_put_word(d,2);
	d+=lw_put_word(d,2);
	//d+=lw_put_byte(d,acinfo->security);
	return d-dst;
}
