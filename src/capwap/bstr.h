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
 * @file
 * @brief Definitions for bstr functions
 */

#ifndef __BSTR_H
#define __BSTR_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * bstr typeS
 *
 * bstr_t serves as binary string where the first byte cponntains
 * the length of the string.
 */
typedef uint8_t* bstr_t;

extern uint8_t * bstr_create(uint8_t *data, uint8_t len);
extern uint8_t * bstr_create_from_cfgstr(const char * s);
extern uint8_t * bstr_replace( bstr_t * dst, uint8_t * bstr);

extern int bstr_to_str(char *dst, bstr_t str,char * def);



/**
 * Return the length of a bstr_t string.
 */
#define bstr_len(s) (*((uint8_t*)(s)))

/**
 * Return the data of a bstr_t string.
 */ 
#define bstr_data(s) ((s)+1)

/**
 * Return the actual size in memory a bstr_t string needs.
 */ 
#define bstr_size(len) (len+1)

/**
 * Max. length of a bstr_t string.
 */ 
#define BSTR_MAX_LEN 254


typedef uint8_t *bstr16_t;
#define bstr16_len(s) ( *((uint16_t*)(s)) )
#define bstr16_data(s) ((s)+2)
#define bstr16_size(l) (l+2)
#define BSTR16_MAX_LEN (0xffff-2)

static inline int bstr16_ncpy(uint8_t *dst,uint8_t*src,uint16_t len)
{
	*((uint16_t*)dst)=len;
	memcpy(dst+2,src,len);
	return len+2;
}

static inline uint8_t * bstr16_create(uint8_t *data, uint16_t len)
{
	uint8_t * str = malloc(2+len*sizeof(uint8_t));
	if (!str)	
		return 0;
	*((uint16_t*)str)=len;
	memcpy(str+2,data,len);
	return str;
}


typedef uint8_t * vendorstr_t;

#define vendorstr_get_vendor_id(str)\
	( *((uint32_t*)((str)+2)))

#define vendorstr_set_vendor_id(str,id)\
	( *((uint32_t*)((str)+2)) = id)

#define vendorstr_len(str)\
	(*((uint16_t*)((str)+0)))

#define vendorstr_set_len(str,len)\
	(*((uint16_t*)((str)+0))=len)

#define vendorstr_data(str)\
	(((uint8_t*)(str))+6)

#define vendorstr_size(n)\
	(1+6+(len)*sizeof(uint8_t))


static inline uint8_t * vendorstr_create(uint32_t vendor_id, uint8_t *data, uint8_t len)
{
	uint8_t * str = malloc(vendorstr_size(len));
	if (!str)	
		return 0;

	vendorstr_set_vendor_id(str,vendor_id);
	vendorstr_set_len(str,len);
	memcpy(vendorstr_data(str),data,len);
	*(vendorstr_data(str)+vendorstr_len(str))=0;
	return str;

}



#endif

