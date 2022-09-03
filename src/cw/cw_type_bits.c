#include "val.h"

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

static int bread(cw_Cfg_t *cfg, const char * key, const uint8_t *src, int len, const void *param)
{
	const struct cw_ValBit * bits=param;
	int l,i;
	
	l = get_len(bits);
	for(i=0;bits[i].key!=NULL;i++){
		int rc;
		printf("%s: %d\n",bits[i].key,get_bit(src,bits[i].bit,l));
	}
	return 1;
}


static 	int bwrite(cw_Cfg_t ** cfgs, const char *key, uint8_t *dst, const void * param)
{
	return 0;
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
