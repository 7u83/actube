
#include <stdint.h>
#include <stdio.h>

#include "conn.h"
#include "capwap.h"
#include "capwap_items.h"

#include "sock.h"
#include "action.h"
#include "cw_log.h"
#include "dbg.h"




int snd_cb(void *c, void *d)
{
	struct conn *conn = (struct conn *) c;
	struct cw_action_in *a = (struct cw_action_in *) d;

	if (a->start) {
		a->start(conn, a, conn->resp_buffer, 0);
	}


	return 1;
}


extern int cw_init_response(struct conn *conn, uint8_t * req);



struct priv {
	struct conn *conn;
	uint8_t *buffer;
	uint32_t msg_id;
	int len;
};

int cw_additems_cb(void *p, void *i)
{
	struct priv *priv = (struct priv *) p;

	struct cw_item *item = (struct cw_item *) i;

	printf("Item: %d - %d \n", priv->msg_id, item->id);

	cw_action_out_t as;
	as.item_id = item->id;
	as.msg_id = priv->msg_id;


	cw_action_out_t *a = cw_actionlist_out_get(priv->conn->actions->out, &as);

	if (!a) {
		printf("No out action found for %d\n", item->id);
		return 1;
	}

	printf("Out action found\n");
	return 1;

}

/*
int cw_additems( struct conn *conn,cw_itemstore_t itemstore)
{
	struct priv priv;
	priv.msg_id=CW_MSG_DISCOVERY_RESPONSE;
	priv.conn=conn;

	avltree_foreach_asc(itemstore, cw_additems_cb,&priv);

}

*/

int cb(void *p, void *ain)
{
	cw_action_out_t *a = (cw_action_out_t *) ain;
	struct priv *priv = (struct priv *) p;
	if (priv->msg_id != a->msg_id)
		return 0;

	if (!a->item_id)
		return 1;

	printf("Put Msg ID %d (%s) - %d\n", a->msg_id, cw_strmsg(a->msg_id), a->elem_id);

	struct cw_item *item = NULL;
	if (a->get)
		item = a->get(priv->conn, a->item_id);

	int l = 0;

	uint8_t *buffer = priv->buffer + priv->len;
	printf("Writing to %p\n", buffer);


	if (a->out) {
		l = a->out(priv->conn, a->elem_id, buffer, item);
		printf("The length is %d\n", l);
	}

	printf("+++++++++++++++++++Adding %d to privlen\n", l);
	priv->len += l;
	printf("Priveln is now %d\n", priv->len);
	return 1;
}



int conn_send_msg(struct conn *conn, uint8_t * rawmsg);

int cw_send_response(struct conn *conn, uint8_t * rawmsg, int len)
{
	struct priv priv;

	cw_init_response(conn, rawmsg);


	uint8_t *msgptr = rawmsg + cw_get_hdr_msg_offset(rawmsg);

	cw_action_out_t as;

	as.msg_id = cw_get_msg_type(msgptr) + 1;
	as.item_id = CW_ITEM_NONE;

	priv.msg_id = as.msg_id;
	priv.conn = conn;
	priv.len = 0;
	priv.buffer = cw_get_hdr_msg_elems_ptr(conn->resp_buffer);

	int off = cw_get_hdr_msg_offset(conn->resp_buffer);
	printf("Offset is %d\n", off);

	avltree_foreach_from_asc(conn->actions->out, &as, cb, &priv);

	uint8_t *smptr = conn->resp_buffer + cw_get_hdr_msg_offset(conn->resp_buffer);
	printf("Send Type: %d\n", cw_get_msg_type(smptr));


	printf("The total msgelems length is %d\n", priv.len);

	int offset = cw_get_hdr_msg_offset(conn->resp_buffer);

	printf("Privlen = %d\n", priv.len);
	cw_set_msg_elems_len(conn->resp_buffer + offset, priv.len);

	int total = cw_get_hdr_msg_total_len(conn->resp_buffer);

	printf("TOTAL lenz: %p %d\n", conn->resp_buffer, total);

	conn_send_msg(conn, conn->resp_buffer);

	return 0;




/*	
printf("The repoinse finder\n");
	
	struct cw_action_in as,*af;
	uint8_t * msg_ptr = rawmsg+cw_get_hdr_msg_offset(rawmsg);
	
	as.msg_id = cw_get_msg_id(msg_ptr)+1;
	if (as.msg_id & 1)
		return 0;

printf("Message ID = %d\n",as.msg_id);

	as.elem_id=0;
	as.vendor_id=0;
	as.proto=1;
	as.capwap_state=1;
	//avltree_foreach_from_asc(conn->msgtr, &as,snd_cb,conn);

	printf("BLA\n");
*/
}


int cw_process_msg(struct conn *conn, uint8_t * rawmsg, int len)
{
	struct cw_action_in as, *af,*afm;

	uint8_t *msg_ptr = rawmsg + cw_get_hdr_msg_offset(rawmsg);

	int elems_len = cw_get_msg_elems_len(msg_ptr);
/*
	if (8+elems_len != len){
		cw_dbg(DBG_MSG_ERR,"Discarding message from %s, msgelems len=%d, data len=%d, (strict capwap) ",
			sock_addr2str(&conn->addr),elems_len,len-8);

		if (conn_is_strict_capwap(conn)){
			cw_dbg(DBG_MSG_ERR,"Discarding message from %s, msgelems len=%d, data len=%d, (strict capwap) ",
				sock_addr2str(&conn->addr),elems_len,len-8);
			return 0;
		}
		if (8+elems_len < len){
			cw_dbg(DBG_CW_RFC,"Packet from from %s has %d bytes extra data.",
				sock_addr2str(&conn->addr),len-8-elems_len);
			elems_len=len-8;
		}

		if (8+elems_len > len){
			cw_dbg(DBG_CW_RFC,"Packet from from %s hass msgelems len of %d bytes but has only %d bytes of data, truncating.",
				sock_addr2str(&conn->addr),elems_len,len-8);
		}
		return 1;
	}

*/


	/* prepare struct for search operation */
	as.capwap_state = conn->capwap_state;
	as.msg_id = cw_get_msg_id(msg_ptr);
	as.vendor_id = 0;
	as.elem_id = -1;
	as.proto=0;


	/* Search for state/message combination */
	afm = cw_actionlist_in_get(conn->actions->in, &as);

	/* Check if message comes in right state */
	if (!afm) {
		cw_dbg(DBG_MSG_ERR, "Message type %d (%s) not allowed in %s State.",
		       as.msg_id, cw_strmsg(as.msg_id), cw_strstate(as.capwap_state));
		return 0;
	}


	/* Execute start processor for message */
	if (afm->start) {
		afm->start(conn, afm, rawmsg, len);
	}

	uint8_t *elems_ptr = cw_get_msg_elems_ptr(msg_ptr);
	uint8_t *elem;

	cw_foreach_elem(elem, elems_ptr, elems_len) {

		as.elem_id = cw_get_elem_id(elem);
		int elem_len = cw_get_elem_len(elem);

		cw_dbg_elem(as.msg_id, as.elem_id, cw_get_elem_data(elem), elem_len);


		af = cw_actionlist_in_get(conn->actions->in, &as);

		if (!af) {
			cw_dbg(DBG_ELEM_ERR, "Element %d not allowed in msg %d (%s)",
			       as.elem_id, as.msg_id, cw_strmsg(as.msg_id));
			continue;
		}

		if (af->start) {
			af->start(conn, af, cw_get_elem_data(elem), elem_len);
		}

	}


	cw_send_response(conn, rawmsg, len);

	if (afm->end) {
		afm->end(conn, afm, rawmsg, len);
	}


	return 0;

}
