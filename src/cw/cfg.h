#ifndef _CFG_H
#define _CFG_H

#include <mavl.h>
#include "val.h"
#include "bstr.h"

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

int cw_cfg_get_bool(cw_Cfg_t * cfg, const char * key, int def);
uint16_t cw_cfg_get_word(cw_Cfg_t * cfg, char *key, uint16_t def);
void cw_cfg_set_int(cw_Cfg_t * cfg, const char * key, int val);
uint8_t cw_cfg_get_byte(cw_Cfg_t * cfg, char *key, uint8_t def);
bstr16_t cw_cfg_get_bstr16(cw_Cfg_t * cfg, const char * key, const char *def);
int cw_cfg_set_bstr16(cw_Cfg_t * cfg, const char * key, bstr16_t str);
int cw_cfg_get_next_index(cw_Cfg_t * cfg, const char *key);
const char *cw_cfg_get_l(cw_Cfg_t ** cfg, const char * key, const char *def);
void cw_cfg_copy(cw_Cfg_t *src, cw_Cfg_t *dst);
void cw_cfg_destroy(cw_Cfg_t *cfg);
void cw_cfg_clear(cw_Cfg_t *cfg);
int cw_cfg_base_exists(cw_Cfg_t * cfg, const char *key);
uint8_t cw_cfg_get_byte_l(cw_Cfg_t ** cfgs, char *key, uint8_t def);
cw_Val_t * cw_cfg_get_val_l(cw_Cfg_t ** cfgs, const char *key, const struct cw_Type *type);
int cw_cfg_base_exists_l(cw_Cfg_t ** cfgs, const char *key);
int cw_cfg_save(const char *filename, cw_Cfg_t *cfg);






int cw_cfg_set_val(cw_Cfg_t * cfg, const char *key, const struct cw_Type *t, const void * valguard, const uint8_t * data, int len);

#define cw_cfg_get_word2(cfg1,cfg2,key,def) \
	cw_cfg_get_word(cfg1,key,cw_cfg_get_word(cfg2,key,def))

#define cw_dget(fun,cfg1,cfg2,key,def)\
	fun(cfg1,key,fun(cfg2,key,def))


#endif

