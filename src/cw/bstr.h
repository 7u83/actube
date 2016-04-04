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
 * @defgroup Bstr BSTR 
 * @brief BSTR is used to store binary strings.
 * We can see them anywhere.
 * @{
 */

#ifndef __BSTR_H
#define __BSTR_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * @defgroup BSTRTypes Types
 * @{
 */

/**
 * bstr type
 *
 * bstr_t serves as binary string, where the first byte contains
 * the length of the string.
 */
typedef uint8_t* bstr_t;

/**
 *@}
 */

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
 *@defgroup BSTRConstants Constants
 *@{
 */

/**
 * Maximum length of a bstr_t string.
 */ 
#define BSTR_MAX_LEN 254

/**@}*/


/**
 *@addtogroup BSTRTypes 
 *@{
 */

/**
  * The same as #bstr_t, but there are two bytes used
  * to describe the length of the string.
  */  
typedef uint8_t *bstr16_t;

/**
 *@}
 */


/**
 * Return the length of a bstr16_t string.
 */ 
#define bstr16_len(s) ( *((uint16_t*)(s)) )
/**
 * Return a pointer to the data of a bstr16_t string.
 */ 
#define bstr16_data(s) (((uint8_t*)s)+2)

/**
 * Return the actual size of a bstr16_t string. That's the 
 * size this objects needs in memory to be stored.
 */ 
#define bstr16_size(l) (l+2)
/**
   Maximum length of a #bstr16_t string 
*/
#define BSTR16_MAX_LEN (0xffff-2)

/*
static inline int bstr16_ncpy(uint8_t *dst,uint8_t*src,uint16_t len)
{
	*((uint16_t*)dst)=len;
	memcpy(dst+2,src,len);
	return len+2;
}
*/


static inline uint8_t * bstr16_create(const uint8_t *data, uint16_t len)
{
	uint8_t * str = malloc(2+len*sizeof(uint8_t));
	if (!str)	
		return 0;
	*((uint16_t*)str)=len;
	memcpy(str+2,data,len);
	return str;
}

uint8_t * bstr16_create_from_str(const char *s);
extern uint8_t * bstr16_create_from_cfgstr(const char * s);

#define bstr16_replace bstr_replace


/**
 *@addtogroup BSTRTypes 
 *@{
 */

/**
 * The bstrv_t type is basicly a #bstr16_t and can be 
 * handeld like a bstr16_t.
 * The difference is, that the first four bytes of the 
 * string data containing a vendor id.
 */
typedef uint8_t * bstrv_t;

/**
 *@}
 */

#define bstrv_get_vendor_id(str)\
	( *((uint32_t*)((str)+2)))

#define bstrv_set_vendor_id(str,id)\
	( *((uint32_t*)((str)+2)) = id)

#define bstrv_len(str)\
	(*((uint16_t*)((str)+0)))

#define bstrv_set_len(str,len)\
	(*((uint16_t*)((str)+0))=len)

#define bstrv_data(str)\
	(((uint8_t*)(str))+6)

#define bstrv_size(n)\
	(1+6+(n)*sizeof(uint8_t))


static inline uint8_t * bstrv_create(uint32_t vendor_id, uint8_t *data, uint8_t len)
{
	uint8_t * str = malloc(bstrv_size(len));
	if (!str)	
		return 0;

	bstrv_set_vendor_id(str,vendor_id);
	bstrv_set_len(str,len);
	memcpy(bstrv_data(str),data,len);
	*(bstrv_data(str)+bstrv_len(str))=0;
	return str;

}

uint8_t * bstrv_create_from_str(uint32_t vendor_id,const char *s);


//uint8_t * bstr16cfgstr(const char * s);


#endif

/**@}*/

