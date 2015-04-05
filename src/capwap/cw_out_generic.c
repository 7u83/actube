


#include "capwap.h"
#include "capwap_items.h"


int cw_put_item(uint8_t *dst,struct cw_item*item)
{
	switch (item->type){
		case CW_ITEMTYPE_STR:
			printf("Outing str %s\n",item->data);
			return cw_put_data(dst,item->data,strlen( (char*)item->data));
		case CW_ITEMTYPE_BYTE:
			return cw_put_byte(dst,item->byte);
		case CW_ITEMTYPE_WORD:
			return cw_put_word(dst,item->word);
		case CW_ITEMTYPE_DWORD:
			return cw_put_word(dst,item->dword);
	}

	return 0;
}

int cw_out_generic(struct conn *conn,uint32_t elem_id,uint8_t *dst,struct cw_item * item) 
{
	int len;
	if ( !item )
		len=0;
	else
		len = cw_put_item(dst+4,item);
	return len + cw_put_elem_hdr(dst,elem_id,len);	
}


