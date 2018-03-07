#ifndef __CW_TYPES_H
#define __CW_TYPES_H

#include "mavl.h"

struct cw_Type{
        /** A human readable name for this type */
        const char *name;

       /** A pointer to a function to delete elements of this type */
        void (*del)(mavldata_t *data);
	
        /** A method to put this object to a buffer */
        int (*put)(mavldata_t * data, uint8_t*dst);
	
	/** The get emthod */
        mavldata_t * (*get)(mavldata_t *data, const uint8_t*src, int len);

        /** A pointer to a function to convert elements of this type to a string.
            This function is mainly used to store elements to an SQL database
            or to json strings */
        int (*to_str)(const mavldata_t * data, char *dst, int max_len);

        /** Cereate an item of this type from a string, which was previously
            created by the #del function. */
	mavldata_t * (*from_str)(mavldata_t * data, const char *src);

/*
        int (*def)(void *, void *);
*/
 

};


extern const struct cw_Type  cw_type_byte;
extern const struct cw_Type  cw_type_word;
extern const struct cw_Type  cw_type_dword;
extern const struct cw_Type  cw_type_version;

#define CW_TYPE_BYTE (&cw_type_byte)
#define CW_TYPE_DWORD (&cw_type_dword)
#define CW_TYPE_VERSION (&cw_type_version)

#endif /* __CW_TYPES_H */
