#include "cw/lw.h"
#include "cw/cw.h"
#include "cw/dbg.h"
#include "cw/cfg.h"

#include "capwap_cisco.h"
#include "mod_cisco.h"



int cisco_out_lw_path_mtu(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)
{

	char key[CW_CFG_MAX_KEY_LEN];
	int len,max;

	int hl = params->msgset->header_len(eh);
	
	/* get the current length of message */
	int cl = dst-params->rawmsg;



	int wl = 1400-cl-hl;

//printf("HL:%d CL:%d WL: %d\n",hl,cl,wl);
	if (wl<=40)
		return 0;

	sprintf(key,"%s/%s",eh->key,"len");
	len = cw_cfg_get_word_l(params->cfg_list,key,0);
	if (len == 0)
		return 0;
	sprintf(key,"%s/%s",eh->key,"max");
	max = cw_cfg_get_word_l(params->cfg_list,key,0);
	
	lw_set_word(dst+hl,max);
	lw_set_word(dst+hl+2,wl);
	memset(dst+hl+4,0,wl-4);

	len = wl;	
	
	return params->msgset->write_header(eh,dst,len);
	
}
