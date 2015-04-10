#ifndef __ACIPLIST_H
#define __ACIPLIST_H

#include "avltree.h"

struct cw_acip{
	struct sockaddr_storage ip;
	uint32_t wtp_count;
};

typedef struct cw_acip cw_acip_t;

typedef struct avltree * cw_aciplist_t;

extern cw_aciplist_t cw_aciplist_create();

#define cw_aciplist_destroy(l) avltree_destroy(l)
#define cw_aciplist_add(l,elem) avltree_add(l,elem)
#define cw_aciplist_del(l,elem) avltree_del(l,elem) 
#define cw_aciplist_foreach(l,callback,cbpriv) avltree_foreach_asc(l,callback,cbpriv)
#define cw_aciplist_replace(l,r) avltree_replace(l,r)

#endif

