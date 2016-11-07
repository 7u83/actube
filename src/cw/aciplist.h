#ifndef __ACIPLIST_H
#define __ACIPLIST_H

#include "mavl.h"

struct cw_acip{
	struct sockaddr_storage ip;
	uint32_t index;
};

typedef struct cw_acip cw_acip_t;

typedef struct mavl * cw_aciplist_t;

extern cw_aciplist_t cw_aciplist_create();

#define cw_aciplist_destroy(l) mavl_destroy(l)
#define cw_aciplist_add(l,elem) mavl_add(l,elem)
#define cw_aciplist_del(l,elem) mavl_del(l,elem) 
#define cw_aciplist_foreach(l,callback,cbpriv) mavl_foreach_asc(l,callback,cbpriv)
#define cw_aciplist_replace(l,r) mavl_replace(l,r)

#endif

