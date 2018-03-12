/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    actube is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "cw/log.h"
#include "cw/conn.h"

#include "cw/mbag.h"
#include "cw/capwap_items.h"
#include "cw/cw.h"

extern mbag_t cisco_config;

int cisco_out_ac_descriptor(struct conn *conn,struct cw_action_out * a,uint8_t *dst) 
{

	uint8_t *d = dst+4;
	struct mbag_item * i;
	i = mbag_get(conn->local,CW_ITEM_AC_STATUS);
	
	if (!i) {
		cw_log(LOG_ERR,"Can't send AC Descriptor, no AC Status Item found");
		return 0; 
	}

	d+=cw_put_ac_status(d ,(struct cw_ac_status*)(i->u2.data),conn);


	i = mbag_get(cisco_config,CW_ITEM_AC_SOFTWARE_VERSION);

	if (!i){
		/* Send back the same software version as the WTP has,
		otherwise the AP wants us to send an image */
		i = mbag_get(conn->incomming,CW_ITEM_WTP_SOFTWARE_VERSION);
	}
/*	
	if ( i ) {	
	 	d += cw_put_version(d,1,i->u2.data);
	}
	else {
		cw_log(LOG_ERR, "Can't set Cisco Software Version in AC descriptor, No value defined.");
	}

	i = mbag_get(cisco_config, CW_ITEM_AC_HARDWARE_VERSION);
	if ( i ) {	
	 	d += cw_put_version(d,0,i->u2.data);
	}
	else {
		cw_log(LOG_ERR, "Can't set Cisco Hardware Version in AC descriptor, No value defined.");
	}
*/

	int len = d-dst-4;
	
	return len + cw_put_elem_hdr(dst,a->elem_id,len);
}



