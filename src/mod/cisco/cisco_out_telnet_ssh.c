
#include "cisco.h"
#include "cw/capwap_items.h"

#include "cw/lwapp.h"
#include "cw/cw.h"
#include "cw/dbg.h"
#include "cw/vendors.h"
#include "cw/lwapp_cisco.h"
#include "cw/capwap_cisco.h"


#include "include/cipwap_items.h"



int lw_cisco_put_telnet_ssh(struct conn *conn, uint8_t type,const char * item_id, uint8_t *dst) 
{

	uint8_t *d = dst+6;

	mbag_item_t * item; 

	item = mbag_get(conn->outgoing, item_id);

	cw_dbg(DBG_X,"Getting an Item %p\n",item);

	if (item){
		if (item->type == MBAG_BYTE) {
			d+=lw_put_byte(d,item->byte);
			d+=lw_put_byte(d,type);
		}
	}

	int l = d-6-dst;

	cw_dbg(DBG_X," the calculated length is %d",l);

	if (l==0)
		return 0;


	return l + lw_put_vendor(dst, LW_VENDOR_ID_CISCO, LW_CISCO_TELNET_SSH, l);

}

int cw_cisco_put_telnet_ssh(struct conn *conn,uint8_t type,const char * item_id, uint8_t *dst) 
{
	int l = lw_cisco_put_telnet_ssh(conn,type,item_id,dst+10);
	if (l==0)
		return 0;
	cw_dbg(DBG_X,"CISL = %d",l);
	return l + cw_put_elem_vendor_hdr(dst, CW_VENDOR_ID_CISCO, CW_CISCO_SPAM_VENDOR_SPECIFIC, l);

}


int cisco_out_telnet_ssh(struct conn *conn,struct cw_action_out * a,uint8_t *dst) 
{
	int l;
	l=cw_cisco_put_telnet_ssh(conn,0,CIPWAP_ITEM_TELNET_ENABLE,dst);
	l+=cw_cisco_put_telnet_ssh(conn,1,CIPWAP_ITEM_SSH_ENABLE,dst+l);
	return l;
	
}

