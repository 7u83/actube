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

#include "cw.h"
#include "dbg.h"

int cw_read_descriptor_subelems(mbag_t store, uint8_t * data, int len,
				struct cw_descriptor_subelem_def *elems)
{
	int errors = 0;
	int success = 0;
	int sub = 0;
	while (sub < len) {

		if (len - sub < 8) {
			return 0;
		}
		uint32_t vendor_id = cw_get_dword(data + sub);
		int sublen = cw_get_word(data + sub + 6);
		int subtype = cw_get_word(data + sub + 4);

		bstrv_t vstr = NULL;
		int i;
		for (i = 0; elems[i].maxlen; i++) {
			if (elems[i].type == subtype && elems[i].vendor_id==vendor_id)
				break;
		}
		if (!elems[i].maxlen) {
			vstr = bstrv_create(vendor_id, data + sub + 8, sublen);
			if (vstr) {
				cw_dbg_version_subelem(DBG_ELEM_ERR, "Not allowed",
						       subtype, vstr);
				free(vstr);
			}
			errors++;
		} else {
			int l = sublen;
			if (elems[i].maxlen < sublen) {
				cw_dbg(DBG_ELEM_ERR,
				       "SubType %d Too long (truncating), len = %d,max. len=%d",
				       subtype, sublen, elems[i].maxlen);
				l = elems[i].maxlen;
			}

			vstr =
			    mbag_set_bstrv(store, elems[i].item_id, vendor_id,
					   data + sub + 8, l);

			char dbgstr[128];
			sprintf(dbgstr, "Storing '%s'", elems[i].item_id);
			cw_dbg_version_subelem(DBG_SUBELEM, dbgstr, subtype, vstr);
			success++;
		}

		if (sub + sublen > len)
			return -1;

		sub += sublen + 8;

	}



	return success;
}
