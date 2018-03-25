#ifndef __KVT_H
#define __KVT_H

#include <stdint.h>
#include <stdio.h>

#include "mavl.h"


/**
 * @addtogroup ALGOS
 * @{
 */


/**
 * @defgroup KTV KTV Store (Key-Type-Value-Store)
 * @{
 */

#define CW_KTV_MAX_KEY_LEN 1024

/**
 * @struct cw_KTV
 * @file ktv.h
 * @brief Structure to store a key-type-value element.
 */
struct cw_KTV {
	/** The key for this element. A string. */
	char *key;
	/** Teh type of this element. */
	const struct cw_Type *type;
	/** The value for this element */
	union {
		uint32_t dword;	
		uint16_t word;
		uint8_t byte;
		void *ptr;
		int boolean;
	} val;
};
typedef struct cw_KTV cw_KTV_t;

/**
 * @class cw_Type
 * @author 7u83
 * @file ktv.h
 * @brief Representation of a cw_Type objetc
 */
struct cw_Type {
	/** A human readable name for this type */
	const char *name;

	/** A pointer to a function to delete elements of this type */
	void (*del) (struct cw_KTV * data);

	/** A method to put this object to a buffer */
	int (*put) (const struct cw_KTV * data, uint8_t * dst);

	/** The get method */
	struct cw_KTV *(*get) (struct cw_KTV * data, const uint8_t * src, int len);

	/** A pointer to a function to convert elements of this type to a string.
	  * This function is mainly used to store elements to an SQL database
	  * or to json strings 
	  * @param data pointer to KTV object
	  * @param dst buffer where to put the string in
	  * @param max_len maximum length of buffer
	  * @return The number of bytes written to dst
	*/
	int (*to_str) (const struct cw_KTV * data, char *dst, int max_len);

	/** Cereate an item of this type from a string, which was previously
	    created by the #del function. */
	struct cw_KTV *(*from_str) (struct cw_KTV * data, const char *src);

	
	int (*len)(cw_KTV_t *);
	
};
typedef struct cw_Type cw_Type_t;


struct cw_KTVStruct {
	const struct cw_Type * type;
	const char * key;
	int len;
	int position;
};
typedef struct cw_KTVStruct cw_KTVStruct_t;
int cw_ktv_read_struct(mavl_t ktv,cw_KTVStruct_t * stru, const char *pkey, 
	uint8_t * data, int len);

extern const struct cw_Type cw_type_byte;
extern const struct cw_Type cw_type_word;
extern const struct cw_Type cw_type_dword;
extern const struct cw_Type cw_type_bstr16;
extern const struct cw_Type cw_type_ipaddress;

#define CW_TYPE_BYTE (&cw_type_byte)
#define CW_TYPE_WORD (&cw_type_word)
#define CW_TYPE_DWORD (&cw_type_dword)
#define CW_TYPE_BSTR16 (&cw_type_bstr16)
#define CW_TYPE_IPADDRESS (&cw_type_ipaddress)

/*
void cw_kvstore_mavl_delete(const void *data);
 */
cw_KTV_t *cw_ktv_add(mavl_t kvstore, const char *key, const struct cw_Type *type,
			   const uint8_t * data, int len);

const char * cw_ktv_add_from_str(mavl_t kvtstore, const char *key, const struct cw_Type *type,
			const char * str);

int cw_ktv_mavlcmp(const void *v1, const void *v2);
int cw_ktv_mavlcmp_type_by_name(const void *v1,const void *v2);

void cw_ktv_mavldel(void *data);

/**
 * Create a KTV store 
 * @return a #mavl_t object representing the KTV store
 */
#define cw_ktv_create()\
	mavl_create(cw_ktv_mavlcmp, cw_ktv_mavldel, sizeof(cw_KTV_t))
	
#define cw_ktv_create_types_tree()\
	mavl_create(cw_ktv_mavlcmp_type_by_name,NULL,sizeof(struct cw_Type *))

int cw_ktv_read_line (FILE *f, char * key, char * type, char *val);
int cw_ktv_read_file(FILE * file, mavl_t ktv, mavl_t types);
cw_KTV_t * cw_ktv_get(mavl_t ktv, const char *key, const cw_Type_t * type);
uint8_t cw_ktv_get_byte(mavl_t ktv,const char *key, uint8_t def);
uint16_t cw_ktv_get_word(mavl_t ktv,const char *key, uint16_t def);

int cw_ktv_idx_get(mavl_t ktv, const char *key, int idx, const cw_Type_t * type);

		
extern const cw_Type_t * cw_ktv_std_types[];
#define CW_KTV_STD_TYPES cw_ktv_std_types

/**
 * @}  KTV
 * @}  ALGOS
 */

#endif	/* __KVT_H */
