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

int cw_put_mbag_item(uint8_t * dst, struct mbag_item *item)
{
	if (MBAG_STR == item->type ){
		return cw_put_data(dst, item->data, strlen((char *) item->data));
	}

	if (MBAG_DATA == item->type){
		return cw_put_data(dst, item->data+1, *((uint8_t*)item->data));
	}

	if (MBAG_BYTE == item->type){
		return cw_put_byte(dst, item->byte);
	}
	if (MBAG_WORD == item->type){
		return cw_put_word(dst, item->word);
	}
	if (MBAG_DWORD == item->type){
		return cw_put_dword(dst, item->dword);
	}
	if (MBAG_BSTR  == item->type) {
		return cw_put_bstr(dst, item->data);
	}

	if ( MBAG_BSTR16 == item->type)
		return cw_put_bstr16(dst,item->data);

	if (MBAG_VENDORSTR == item->type)
	{
		int l=0;
		l+=cw_put_dword(dst, bstrv_get_vendor_id(item->data));
		l+=cw_put_data(dst+4, bstrv_data(item->data),bstrv_len(item->data));
		return l;
	}

	
	if (MBAG_MBAG == item->type){
		*((void**)dst)=item->data;
		return sizeof(void *);
	}

	cw_log(LOG_ERR,"No method to put items of type %s",item->type->name);


	return 0;
}


