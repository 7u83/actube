
#include "cw.h"
#include "dbg.h"
#include "cfg.h"

int cw_out_generic_with_index(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)

{
	int len,start;
	uint8_t * ob;
	struct cw_Cfg_entry *e;
	struct cw_Cfg_iter cfi;
	struct cw_Type * type = (struct cw_Type *)eh->type;
	int i,l;


	ob = dst;

	cw_cfg_iter_init(params->cfg_list[0], &cfi, eh->key);
	for (i=0; (e = cw_cfg_iter_next(&cfi, NULL)) != NULL; i++){
		start = params->msgset->header_len(eh);
		len = cw_put_byte(ob+start,i);

		cw_dbg(DBG_X,"KV: %s -  %s",e->key,e->val);
//		int bwrite(cw_Cfg_t ** cfgs, const char *key, uint8_t *dst, const void * param)
		l = type->write(params->cfg_list,e->key,ob+start+len,eh->param);
		if (l==-1)
			continue;
		len += l;

		//len += result->type->put(result,ob+start+len);

		ob += params->msgset->write_header(eh,ob,len);
		

	}

	return ob-dst;
	
}

