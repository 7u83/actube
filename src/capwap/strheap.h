#ifndef __STRHEAP_H
#define __STRHEAP_H

#include <stdint.h>

#include "avltree.h"


typedef struct avltree * cw_strheap_t;

struct cw_str {
	uint32_t id;
	const char *str;
};

extern cw_strheap_t cw_strheap_create();
extern int cw_strheap_register_strings(cw_strheap_t h, struct cw_str *s);

static inline const char * cw_strheap_get(cw_strheap_t h, int id) {
	struct cw_str s;
	s.id=id;
	struct cw_str *r = avltree_get(h,&s);
	if (r) 
		return r->str;
	return NULL;
}


#endif
