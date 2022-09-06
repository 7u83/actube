#include "val.h"
#include "cfg.h"
#include "dbg.h"

static int get_len(const struct cw_ValBit *bits)
{
	int i;
	for (i=0;bits[i].key!=NULL;i++);
	return bits[i].bit; 
}

static int get_bit(const uint8_t * src,int pos, int len)
{
	int b;
	uint8_t m;
       	b = len-1-pos/8;
	m = 1<<(pos%8);
	return src[b]&m ? 1:0;
}

static void set_bit(uint8_t * dst,int pos, int len,int val)
{
	int b;
	uint8_t m;
	cw_dbg(DBG_X,"set bit val %d",val);
	if (!val)
		return;
       	b = len-1-pos/8;
	m = 1<<(pos%8);
	dst[b]|=m;
}

static int bread(cw_Cfg_t *cfg, const char * key, const uint8_t *src, int len, const void *param)
{
	char skey[CW_CFG_MAX_KEY_LEN];
	const struct cw_ValBit * bits=param;
	int l,i;
	
	l = get_len(bits);
	for(i=0;bits[i].key!=NULL;i++){
		sprintf(skey,"%s/%s",key,bits[i].key);
		cw_cfg_set_bool(cfg,skey,get_bit(src,bits[i].bit,l));
	}
	return 1;
}


static 	int bwrite(cw_Cfg_t ** cfgs, const char *key, uint8_t *dst, const void * param)
{
	int l,i;
	char skey[CW_CFG_MAX_KEY_LEN];
	const struct cw_ValBit * bits=param;
	l = get_len(bits);
	memset(dst,0,l);
	for(i=0;bits[i].key!=NULL;i++){
		uint8_t val;
		int rc;
		val=0;
		sprintf(skey,"%s/%s",key,bits[i].key);
		rc = CW_TYPE_BOOL->write(cfgs, skey, &val, NULL);
		if (rc<0)
			val=0;
		cw_dbg(DBG_X,"%s (rc: %d)",skey,rc);
		set_bit(dst,bits[i].bit,l,val);
	}
	return l;
}


const struct cw_Type cw_type_bits = {
	"Bits",			/* name */
	NULL,			/* del */
	NULL,			/* put */
	NULL,			/* get */
	NULL,			/* to_str */
	NULL,		/* from_str */ 
	NULL,			/* len */
	NULL,			/* data */
	NULL,		/* get_type_name */
	NULL,
	bread,
	bwrite

};
