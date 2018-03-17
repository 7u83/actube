#ifndef __KVT_H
#define __KVT_H

#include <stdint.h>
#include <stdio.h>

#include "mavl.h"

#define CW_KTV_MAX_KEY_LEN 1024

struct cw_KTV {
	char *key;
	const struct cw_Type *type;
	union {
		uint32_t dword;
		uint16_t word;
		uint8_t byte;
		void *ptr;
		int boolean;
	} val;
};
typedef struct cw_KTV cw_KTV_t;


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
	    This function is mainly used to store elements to an SQL database
	    or to json strings */
	int (*to_str) (const struct cw_KTV * data, char *dst, int max_len);

	/** Cereate an item of this type from a string, which was previously
	    created by the #del function. */
	struct cw_KTV *(*from_str) (struct cw_KTV * data, const char *src);

	
	int (*len)(cw_KTV_t *);
	
};
typedef struct cw_Type cw_Type_t;

extern const struct cw_Type cw_type_byte;
extern const struct cw_Type cw_type_word;
extern const struct cw_Type cw_type_dword;
extern const struct cw_Type cw_type_bstr16;

#define CW_TYPE_BYTE (&cw_type_byte)
#define CW_TYPE_WORD (&cw_type_word)
#define CW_TYPE_DWORD (&cw_type_dword)
#define CW_TYPE_BSTR16 (&cw_type_bstr16)

/*
void cw_kvstore_mavl_delete(const void *data);
 */
const char *cw_ktv_add(mavl_t kvstore, const char *key, const struct cw_Type *type,
			   const uint8_t * data, int len);

const char * cw_ktv_add_from_str(mavl_t kvtstore, const char *key, const struct cw_Type *type,
			const char * str);

int cw_ktv_mavlcmp(const void *v1, const void *v2);
int cw_ktv_mavlcmp_type_by_name(const void *v1,const void *v2);

void cw_ktv_mavldel(void *data);

#define cw_ktv_create()\
	mavl_create(cw_ktv_mavlcmp, cw_ktv_mavldel, sizeof(cw_KTV_t))
	
#define cw_ktv_create_types_tree()\
	mavl_create(cw_ktv_mavlcmp_type_by_name,NULL,sizeof(struct cw_Type *))

int cw_ktv_read_line (FILE *f, char * key, char * type, char *val);
int cw_ktv_read_file(FILE * file, mavl_t ktv, mavl_t types);
cw_KTV_t * cw_ktv_get(mavl_t ktv, const char *key, const cw_Type_t * type);
uint8_t cw_ktv_get_byte(mavl_t ktv,const char *key, uint8_t def);
uint16_t cw_ktv_get_word(mavl_t ktv,const char *key, uint16_t def);

void cw_ktv_dump(mavl_t ktv, uint32_t dbglevel, 
		const char *header, const char *prefix, const char *footer );
		
extern const cw_Type_t * cw_ktv_std_types[];
#define CW_KTV_STD_TYPES cw_ktv_std_types


#endif	/* __KVT_H */
