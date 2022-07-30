#ifndef _CFG_H
#define _CFG_H

#include <mavl.h>

typedef struct mavl cw_Cfg_t;

cw_Cfg_t * cfg_create();
int cfg_set(cw_Cfg_t *cfg,const char *key, const char *val);
void cw_Cfg_dump(cw_Cfg_t *cfg);

#endif

