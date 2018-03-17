#ifndef __STRLIST_H
#define __STRLIST_H

/**
 * @file
 * @Header file for strlist functions.
 */ 


/** 
 * Key/String pair, used for string tables, 
 * to display message element names, help texts 
 * and so on...
 */
struct cw_StrListElem {
	int id; 		/**< ID, an integer value */
	const char *str; 	/**< a pointer to the string */
	const char *descr; 	/**< Description */
};

typedef struct cw_StrListElem* cw_StrList_t;


/** Stopper, indicates the last element in a strlist */
#define CW_STR_STOP	0xffffffff


extern const char *cw_strlist_get_str(struct cw_StrListElem *s, int id);
extern int cw_strlist_get_id(struct cw_StrListElem *s, const char *str);

#endif

