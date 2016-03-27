/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
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

#include "log.h"

#include "capwap_items.h"
#include "action.h"

#include "cw.h"


int cw_out_wtp_reboot_statistics(struct conn *conn, struct cw_action_out *a,
				 uint8_t * dst)
{

	if (!a->get) {
		cw_log(LOG_ERROR, "Can't set WTP resboot stats. No get method");
		return 0;
	}


	struct mbag_item *i = a->get(conn, a);


	if (!i) {
		if (a->mand) {
			cw_log(LOG_ERR,
			       "Can't put mandatory element WTP_REBOOT_STATISTICS");
			return 0;
		}
		return 0;
	}

	mbag_t rs = (mbag_t) i->data;

	uint8_t *d = dst + 4;
	d += cw_put_word(d, mbag_get_word(rs, CW_ITEM_REBOOT_COUNT, 0));
	d += cw_put_word(d, mbag_get_word(rs, CW_ITEM_REBOOT_AC_INITIATED_COUNT, 0));
	d += cw_put_word(d, mbag_get_word(rs, CW_ITEM_REBOOT_LINK_FAILURE_COUNT, 0));
	d += cw_put_word(d, mbag_get_word(rs, CW_ITEM_REBOOT_SW_FAILURE_COUNT, 0));
	d += cw_put_word(d, mbag_get_word(rs, CW_ITEM_REBOOT_HW_FAILURE_COUNT, 0));
	d += cw_put_word(d, mbag_get_word(rs, CW_ITEM_REBOOT_OTHER_FAILURE_COUNT, 0));
	d += cw_put_word(d, mbag_get_word(rs, CW_ITEM_REBOOT_UNKNOWN_FAILURE_COUNT, 0));
	d += cw_put_byte(d, mbag_get_byte(rs, CW_ITEM_REBOOT_LAST_FAILURE_TYPE, 255));

	int l = d - dst - 4;
	return l + cw_put_elem_hdr(dst, a->elem_id, l);
}
