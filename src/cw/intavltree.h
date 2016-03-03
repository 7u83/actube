#ifndef __INTAVLTREE_H
#define __INTAVLTREE_H

#include "mavl.h"

extern struct mavl * intavltree_create();
typedef struct mavl * intavltree_t;
int * intavltree_add(struct mavl * t, int val);

#define intavltree_destroy(t) mavl_destroy(t)
#define intavltree_foreach_asc(t,f,p) mavl_foreach_asc(t,f,p)
#define intavltree_foreach_desc(t,f,p) mavl_foreach_desc(t,f,p)


#endif
