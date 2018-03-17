#ifndef __STRHEAP_H
#define __STRHEAP_H

#include <stdint.h>

#include "strlist.h"
#include "mavl.h"


typedef struct mavl * cw_strheap_t;


extern cw_strheap_t cw_strheap_create();
extern int cw_strheap_register_strings(cw_strheap_t h, struct cw_StrListElem *s);

const char * cw_strheap_get(cw_strheap_t h, int id); 

#endif
