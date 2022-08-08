#ifndef _CFG_H
#define _CFG_H

#include <mavl.h>

#define CW_CFG_MAX_KEY_LEN 1024

typedef struct mavl cw_Cfg_t;

cw_Cfg_t * cw_cfg_create();
int cw_cfg_set(cw_Cfg_t *cfg,const char *key, const char *val);
void cw_cfg_dump(cw_Cfg_t *cfg);
int cw_cfg_read_from_file(FILE * file, cw_Cfg_t * cfg);
int cw_cfg_load(const char *filename,cw_Cfg_t * cfg);

struct cw_Cfg_entry{
	const char *key;
	const char *val;
};


struct cw_Cfg_iter{
	struct mavliter it;
	const char *base;
};

const char * cw_cfg_get(cw_Cfg_t * cfg, const char *key, const char *def);

const char *cw_cfg_iter_next(struct cw_Cfg_iter *cfi, const char *key);
void cw_cfg_iter_init(cw_Cfg_t * cfg, struct cw_Cfg_iter *cfi, const char *base);

int cw_cfg_get_bool(cw_Cfg_t * cfg, const char * key, const char *def);
uint16_t cw_cfg_get_word(cw_Cfg_t * cfg, char *key, const char * def);


#endif

