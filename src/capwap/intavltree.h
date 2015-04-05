#ifndef __INTAVLTREE_H
#define __INTAVLTREE_H

#include "avltree.h"

extern struct avltree * intavltree_create();
int * intavltree_add(struct avltree * t, int val);

#define intavltree_destroy(t) avltree_destroy(t)
#define intavltree_foreach_asc(t,f,p) avltree_foreach_asc(t,f,p)
#define intavltree_foreach_desc(t,f,p) avltree_foreach_desc(t,f,p)


#endif
