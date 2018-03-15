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

/**
 * @file
 */

#include "cw.h"
#include "log.h"
#include "dbg.h"

#include "ktv.h"

int cw_put_mbag_item(uint8_t * dst, struct mbag_item *item)
{
	cw_KTV_t ktv;
	ktv.type->put(&ktv,dst);
/*	
	if (item->type->put){
		cw_dbg(DBG_X,"User put method to put ");
		return item->type->put(item,dst);
	}


	if (MBAG_STR == item->type ){
		return cw_put_data(dst, item->u2.data, strlen((char *) item->u2.data));
	}

	if (MBAG_DATA == item->type){
		return cw_put_data(dst, item->u2.data+1, *((uint8_t*)item->u2.data));
	}

	if (MBAG_BYTE == item->type){
		return cw_put_byte(dst, item->u2.byte);
	}
	if (MBAG_WORD == item->type){
		return cw_put_word(dst, item->u2.word);
	}
	if (MTYPE_DWORD == item->type){
		return cw_put_dword(dst, item->u2.dword);
	}
	if (MBAG_BSTR  == item->type) {
		return cw_put_bstr(dst, item->u2.data);
	}

	if ( MBAG_BSTR16 == item->type)
		return cw_put_bstr16(dst,item->u2.data);

	if (MBAG_VENDORSTR == item->type)
	{
		int l=0;
		l+=cw_put_dword(dst, bstrv_get_vendor_id(item->u2.data));
		l+=cw_put_data(dst+4, bstrv_data(item->u2.data),bstrv_len(item->u2.data));
		return l;
	}

	
	if (MBAG_MBAG == item->type){
		*((void**)dst)=item->u2.data;
		return sizeof(void *);
	}

	cw_log(LOG_ERR,"No method to put items of type %s",item->type->name);

*/
	return 0;
}


