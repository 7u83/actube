#ifndef _CFG_H
#define _CFG_H

#include <mavl.h>
#include "val.h"
#include "bstr.h"


/**
 *@file
 *@brief
 *@defgroup CFG SOCK
 *@{
 */


/** Maximum size of a key used in cfg objects */
#define CW_CFG_MAX_KEY_LEN 1024	

/** Default name for fresh cfg's created by #cw_cfg_create */ 
#define CW_CFG_DEFAULT_NAME "[anonymous]"


/**
 * A Cfg object 
 */
struct cw_Cfg {
	struct mavl * cfg;	/**< The AVL-tree containig the keys 
				     and vals */
	const char *name;	/**< A name for this config object */
	int dbg_level;
	const char *dbg_prefix;
};

typedef struct cw_Cfg cw_Cfg_t;


/**
 * An antry for a Cfg object
 */
struct cw_Cfg_entry{
	const char *key;	/**< A string representing the key 
				     of this entry */
	const char *val;	/**< The value, represented by a string */
};



cw_Cfg_t * cw_cfg_create();
int cw_cfg_set(cw_Cfg_t *cfg,const char *key, const char *val);
void cw_cfg_dump(cw_Cfg_t *cfg);
int cw_cfg_read_from_file(FILE * file, cw_Cfg_t * cfg);
int cw_cfg_load(const char *filename,cw_Cfg_t * cfg);


struct cw_Cfg_iter{
	struct mavliter it;
	const char *base;
};

const char * cw_cfg_get(cw_Cfg_t * cfg, const char *key, const char *def);

struct cw_Cfg_entry *cw_cfg_iter_next(struct cw_Cfg_iter *cfi, const char *key);
void cw_cfg_iter_init(cw_Cfg_t * cfg, struct cw_Cfg_iter *cfi, const char *base);

int cw_cfg_get_bool(cw_Cfg_t * cfg, const char * key, int def);
uint16_t cw_cfg_get_word(cw_Cfg_t * cfg, const char *key, uint16_t def);
void cw_cfg_set_int(cw_Cfg_t * cfg, const char * key, int val);
uint8_t cw_cfg_get_byte(cw_Cfg_t * cfg, char *key, uint8_t def);
bstr16_t cw_cfg_get_bstr16(cw_Cfg_t * cfg, const char * key, const char *def);
int cw_cfg_set_bstr16(cw_Cfg_t * cfg, const char * key, bstr16_t str);
int cw_cfg_get_new_index(cw_Cfg_t * cfg, const char *key);
const char *cw_cfg_get_l(cw_Cfg_t ** cfg, const char * key, const char *def);
void cw_cfg_copy(cw_Cfg_t *src, cw_Cfg_t *dst,int dbg_level,const char *dbg_prefix);
void cw_cfg_destroy(cw_Cfg_t *cfg);
void cw_cfg_clear(cw_Cfg_t *cfg);
int cw_cfg_base_exists(cw_Cfg_t * cfg, const char *key);
uint8_t cw_cfg_get_byte_l(cw_Cfg_t ** cfgs, char *key, uint8_t def);
cw_Val_t * cw_cfg_get_val_l(cw_Cfg_t ** cfgs, const char *key, const struct cw_Type *type);
int cw_cfg_base_exists_l(cw_Cfg_t ** cfgs, const char *key);
int cw_cfg_save(const char *filename, cw_Cfg_t *cfg, const char *format, ...);

uint16_t cw_cfg_get_word_l(cw_Cfg_t ** cfg, const char *key, uint16_t def);
void cw_cfg_fdump(FILE *f, cw_Cfg_t * cfg, const char *filter);

int cw_cfg_read_from_string(const char *str, cw_Cfg_t *cfg);
void cw_cfg_del(cw_Cfg_t * cfg, const char *key);

int cw_cfg_get_first_index(cw_Cfg_t * cfg, const char *key, int n);
int cw_cfg_get_first_index_l(cw_Cfg_t ** cfgs, const char *key, int n);




int cw_cfg_set_val(cw_Cfg_t * cfg, const char *key, const struct cw_Type *t, const void * valguard, const uint8_t * data, int len);

#define cw_cfg_get_word2(cfg1,cfg2,key,def) \
	cw_cfg_get_word(cfg1,key,cw_cfg_get_word(cfg2,key,def))

#define cw_dget(fun,cfg1,cfg2,key,def)\
	fun(cfg1,key,fun(cfg2,key,def))


#endif


/**@}*/

