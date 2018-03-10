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
#include "kvt.h"
#include "dbg.h"
#include "keys.h"


int cw_read_descriptor_subelems(mavl_t cfg, const char * parent_key, 
				uint8_t * data, int len,
				struct cw_DescriptorSubelemDef *elems)
{
	uint32_t vendor_id;
	int sublen,subtype;
	int errors = 0;
	int success = 0;
	int sub = 0;
	while (sub < len) {
		int i;
		
		if (len - sub < 8) {
			return 0;
		}
		vendor_id = cw_get_dword(data + sub);
		sublen = cw_get_word(data + sub + 6);
		subtype = cw_get_word(data + sub + 4);

		
		/* search sub-element */
		for (i = 0; elems[i].maxlen; i++) {
			if (elems[i].type == subtype && elems[i].vendor_id==vendor_id)
				break;
		}
		
		
		if (!elems[i].maxlen) {
			cw_dbg_version_subelem(DBG_ELEM_ERR, "Not allowed",
				subtype, vendor_id, data+sub+8, sublen);
			errors++;
		} else {
			int l = sublen;

			char dbgstr[1048];
			char key[1024];
			
			if (elems[i].maxlen < sublen) {
				cw_dbg(DBG_ELEM_ERR,
				       "SubType %d Too long (truncating), len = %d,max. len=%d",
				       subtype, sublen, elems[i].maxlen);
				l = elems[i].maxlen;
			}


			/* vendor */
			sprintf(key,"%s/%s/%s",parent_key,elems[i].key,CW_KEY_VENDOR);
			cw_kvt_add(cfg,key,CW_TYPE_DWORD,data + sub,4);
	
			/* version */
			sprintf(key,"%s/%s/%s",parent_key,elems[i].key,CW_KEY_VERSION);
			cw_kvt_add(cfg,key,CW_TYPE_BSTR16,data+sub+8,l);
	
			sprintf(dbgstr, "%s", key);
			cw_dbg_version_subelem(DBG_SUBELEM, dbgstr, subtype, vendor_id, data+sub+8,l);
			success++;
		}

		if (sub + sublen > len)
			return -1;

		sub += sublen + 8;

	}

	return success;
}
