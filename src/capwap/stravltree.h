#ifndef __STRAVLTREE_H
#define __STRAVLTREE_H

#include "avltree.h"

extern struct avltree * stravltree_create();
const char * stravltree_add(struct avltree * t, const char * str);
#define stravltree_destroy(t) avltree_destroy(t)
#define stravltree_foreach(t,f,p,d) avltree_foreach(t,f,p,d)


#endif
