#ifndef __KVT_H
#define __KVT_H

#include <stdint.h>

#include "mavl.h"

struct cw_KVT {
	char *key;
	union {
		uint32_t dword;
		uint16_t word;
		uint8_t byte;
		void *ptr;
	} val;
	const struct cw_Type *type;
};
typedef struct cw_KVT cw_KVT_t;


struct cw_Type {
	/** A human readable name for this type */
	const char *name;

	/** A pointer to a function to delete elements of this type */
	void (*del) (struct cw_KVT * data);

	/** A method to put this object to a buffer */
	int (*put) (const struct cw_KVT * data, uint8_t * dst);

	/** The get method */
	struct cw_KVT *(*get) (struct cw_KVT * data, const uint8_t * src, int len);

	/** A pointer to a function to convert elements of this type to a string.
	    This function is mainly used to store elements to an SQL database
	    or to json strings */
	int (*to_str) (const struct cw_KVT * data, char *dst, int max_len);

	/** Cereate an item of this type from a string, which was previously
	    created by the #del function. */
	struct cw_KVT *(*from_str) (struct cw_KVT * data, const char *src);

	/*
	   int (*def)(void *, void *);
	 */
};

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
const char *cw_kvt_add(mavl_t kvstore, const char *key, const struct cw_Type *type,
			   const uint8_t * data, int len);

int cw_kvt_mavlcmp(const void *v1, const void *v2);
void cw_kvt_mavldel(void *data);

#define cw_kvt_create()\
	mavl_create(cw_kvt_mavlcmp, cw_kvt_mavldel, sizeof(cw_KVT_t))

#endif	/* __KVT_H */
