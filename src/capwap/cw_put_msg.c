/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

/**
 * @file
 * @brief implementation of cw_put_msg.
 */ 

#include "capwap.h"
#include "capwap_items.h"
#include "conn.h"

#include "cw_log.h"

struct args{
	struct conn * conn;
	uint32_t msg_id;
	uint8_t * dst;
	int len;
};



static int action_cb(void *args_param, void *a_param)
{
	struct cw_action_out * a = (struct cw_action_out *)a_param;
	struct args * args = (struct args *)args_param;
	
//printf("ACTION_CB: %d %d %d\n",a->msg_id, a->vendor_id, a->item_id);

	if (a->msg_id != args->msg_id) {
		/* Element is from next msg, close action */
		return 0;
	}

	if (a->item_id == CW_ITEM_NONE) {
		/* Start of message */
		args->len=0;
		return 1;		
	}


	int l = 0;

	uint8_t *dst = args->dst + args->len;


	if (a->out) {
		l = a->out(args->conn, a, dst); 
	}

	args->len += l;
	return 1;
}


/**
 * Put a message to a buffer
 * This functions assumes, that a message header with is
 * alread initilaized in buffer 
 * Message alements are taken fom actiondef in #conn->action
 */
int cw_put_msg(struct conn *conn, uint8_t * rawout)
{

	/* rawout is already initialized, so we can get 
	   msg type from buffer */
	uint8_t *msgptr = rawout + cw_get_hdr_msg_offset(rawout);


	/* create search paramaters */
	cw_action_out_t as;

	as.msg_id = cw_get_msg_type(msgptr);
	as.item_id = CW_ITEM_NONE;
	as.vendor_id = 0;

	
	/* setup arguments for callback function */	
	struct args args;

	args.conn = conn;
	args.msg_id = as.msg_id;
	args.dst = msgptr+8; 
	args.len=-1;

	avltree_foreach_from_asc(conn->actions->out, &as, action_cb, &args);

	cw_set_msg_elems_len(msgptr, args.len);

/*
	printf ("Total elems len = %d\n",args.len);
	printf("Total msg len = %d\n",cw_get_hdr_msg_total_len(rawout)); 	
*/

	if (args.len==-1) {
		cw_log(LOG_ERR,"Error: Can't create message of type %d (%s) - no definition found.",
			args.msg_id,cw_strmsg(args.msg_id));

	}

	return args.len;
}




