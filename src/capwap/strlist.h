#ifndef __STRLIST_H
#define __STRLIST_H

/**
 * @file
 * @Header file for strlist functions.
 */ 


/** 
 *Key/String pair, used for string tables, 
 *to display message element names and so on...
 */
struct cw_strlist_elem {
	/** ID, an integer value */
	int id;
	/** a pointer to the string */
	const char *str;
};

typedef struct cw_strlist_elem* cw_strlist_t;


/** Stopper, indicates the last element in a strlist */
#define CW_STR_STOP	0xffffffff


extern const char *cw_strlist_get_str(struct cw_strlist_elem *s, int id);
extern int cw_strlist_get_id(struct cw_strlist_elem *s, const char *str);

#endif

