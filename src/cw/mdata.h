#ifndef __MDATA_H
#define __MDATA_H


#include <stdint.h>

union mdata_ElemData {
	void *raw;
	uint8_t byte;
	uint16_t word;
	uint32_t dword;
	uint64_t qword;
};

struct mdata_Elem{
	const char * key;
	const struct mdata_Type * type;

	union mdata_ElemData data;

};

struct mdata_Type{
        /** A human readable name for this type */
        const char *name;

       /** A pointer to a function to delete elements of this type */
        void (*del)(void*);
	
        /** A method to put this object to a buffer */
        int (*put)(struct mdata_Elem *i,uint8_t*dst);
	
	/** The get emthod */
        struct mdata_Elem * (*get)(const uint8_t*src, int len);

        /** A pointer to a function to convert elements of this type to a string.
            This function is mainly used to store elements to an SQL database
            or to json strings */
        int (*to_str)(const struct mdata_Elem * e,char *dst);

        /** Cereate an item of this type from a string, which was previously
            created by the #del function. */
	struct mdata_Elem * (*from_str)(const char *src);

/*
        int (*def)(void *, void *);
*/
 

};


struct mdata_Elem *mdata_elem_new( const struct mdata_Type * type );


#endif /* __MDATA_H */

