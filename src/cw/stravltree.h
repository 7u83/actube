#ifndef __STRAVLTREE_H
#define __STRAVLTREE_H

#include "avltree.h"

extern struct avltree * stravltree_create();
const char * stravltree_add(struct avltree * t, const char * str);
#define stravltree_destroy(t) avltree_destroy(t)
#define stravltree_foreach_asc(t,f,p) avltree_foreach_asc(t,f,p)
#define stravltree_foreach_desc(t,f,p) avltree_foreach_desc(t,f,p)


#endif
