#ifndef __STRLIST_H
#define __STRLIST_H

/**
 * @file
 * @Header file for strlist functions.
 */ 


#include "stdio.h"

/** Key/String pair, used for string tables. */
struct cw_str {
	int id;
	const char *str;
};

/** Stopper, indicates the laste element in a strlist */
#define CW_STR_STOP	0xffffffff


extern const char *cw_strlist_get_str(struct cw_str *s, int id);
extern int cw_strlist_get_id(struct cw_str *s, const char *str);

#endif

