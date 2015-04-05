


#include "capwap.h"
#include "capwap_items.h"


int cw_put_item(uint8_t *dst,struct cw_item*item)
{
	switch (item->type){
		case CW_ITEMTYPE_STR:
			return cw_put_data(dst,item->data,strlen( (char*)item->data));
		case CW_ITEMTYPE_BYTE:
			return cw_put_byte(dst,item->byte);
		case CW_ITEMTYPE_WORD:
			return cw_put_word(dst,item->word);
		case CW_ITEMTYPE_DWORD:
			return cw_put_dword(dst,item->dword);
	}

	return 0;
}

int cw_out_generic(struct conn *conn,struct cw_action_out *a,uint8_t *dst) // ,struct cw_item * item) 
{


	/* Get the item to put */
	struct cw_item *item=NULL;
	if (a->get){
		item = a->get (conn,a);
	}

	printf("Out generic item %p\n",item);

	/* Size for msg elem header depends on 
	   vendor specific payload */
	int start = a->vendor_id ? 10:4;
		

	int len;
	if ( !item )
		len=0;
	else{
		printf("Putting it\n");
		printf("To put: %d %s\n",len,item->data);
		len = cw_put_item(dst+start,item);
		printf("Putted: %d %s\n",len,item->data);
	}


	if (a->vendor_id) 
		return len + cw_put_elem_vendor_hdr(dst,a->vendor_id,a->elem_id,len);

	return len + cw_put_elem_hdr(dst,a->elem_id,len);	
}


