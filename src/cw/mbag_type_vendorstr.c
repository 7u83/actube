/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
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
 *@file 
 *@brief Implementation of mbag_type bstrv
 *@addtogroup MbagFunctions
 *@{
 */

#include "mbag.h"
#include "format.h"


static int to_str(void *item,char *dst)
{

	mbag_item_t *i= item;
	
	char *d=dst;
	d+=sprintf(d,"%d,",bstrv_get_vendor_id(i->data));

	if (format_is_utf8(bstrv_data(i->data), bstrv_len(i->data))) {
		d += sprintf(d, "%.*s", bstrv_len(i->data),
			     bstrv_data(i->data));
	} else {
		d += sprintf(d, ".x");
		d += format_hex(d, bstrv_data(i->data), bstrv_len(i->data));
	}

	return d-dst;
}


static struct mbag_item *  from_str(const char *src)
{
	mbag_item_t * i = mbag_item_new(MBAG_VENDORSTR);
	if (!i)
		return NULL;

	uint32_t vendor_id=atoi(src);
	const char *s = strchr(src,',');


	if (s){
		i->data=bstrv_create_from_str(vendor_id,s+1);	
	}
	else{
		i->data=bstrv_create_from_str(vendor_id,"");	
	}

	return i;
}

/**
 * Defines the VendorStr type. 
 * 
 * MBAG items of this type containing a variable of type #bstrv_t.
 */
const struct mbag_typedef mbag_type_bstrv = {
	"VendorStr",free,to_str,from_str
};


/**@}*/
