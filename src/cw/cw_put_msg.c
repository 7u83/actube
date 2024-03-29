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


/**
 * Put a message to a buffer
 * This functions assumes, that a message header with is
 * alread initilaized in buffer 
 * Message alements are taken fom actiondef in #conn->action
 */
 
 
int cw_put_custom_msg(struct cw_Conn *conn, uint8_t * rawout, /*mavl_conststr_t elems*/ int t)
{
	
	/*
	// rawout is already initialized, so we can get 
	   //msg type from buffer 
	uint8_t *msgptr = rawout + cw_get_hdr_msg_offset(rawout);
	int msg_id = cw_get_msg_type(msgptr);

	MAVLITER_DEFINE(it,elems);
	mavliter_foreach(&it){
		const char *i= mavliter_get(&it);
		// preapare action search 
		cw_action_out_t as;
		as.msg_id=msg_id;
		as.item_id=i;
		as.vendor_id=0;
	}









	cw_action_out_t as;

	as.item_id = CW_ITEM_NONE;
	as.vendor_id = 0;

	uint8_t *dst = msgptr+8;

/// TODO XXXX
//	DEFINE_AVLITER(i,conn->actions->out);
MAVLITER_DEFINE(i,0);
	cw_action_out_t *am;

	if (! (am=mavliter_seek(&i,&as))){
		cw_log(LOG_ERR,"Error: Can't create message of type %d (%s) - no definition found.",
			as.msg_id,cw_strmsg(as.msg_id));
		return -1;
	}

	cw_action_out_t *ae;
	int len = 0;
	while(NULL != (ae=mavliter_next(&i))) {

//		DBGX("Put %d %i %p\n",ae->msg_id,ae->elem_id,ae->item_id);
//		DBGX("Elem ID %s",ae->item_id);




		
		if ( ae->item_id ) {
//			DBGX("Item ID: %s %p",ae->item_id,CW_ITEM_NONE);
		}

		if (ae->msg_id != as.msg_id) {
			// Element is from next msg, close action 
			break;
		}
		int l=0;
		if (ae->out) {
			l= ae->out(conn, ae, dst+len); 
			len +=l;
//			cw_dbg_elem_colored(DBG_ELEM, conn, ae->msg_id, ae->elem_id,
//				 dst+len-l,l);

		}

//		printf("DEBUGGER\n");
//		cw_dbg_elem(DBG_ELEM, conn, ae->msg_id, ae->elem_id, 0,l);
		
		
//cw_dbg_elem_colored(DBG_ELEM,"Adding element %d to msg %d, len = %d",ae->msg_id,ae->elem_id,l);

	};


	cw_set_msg_elems_len(msgptr, len);

	if (as.msg_id & 1) {
		/// It's a request, so we have to set seqnum 
		int s = conn_get_next_seqnum(conn);
		cw_set_msg_seqnum(msgptr,s);	
//		printf("Set seqnum to : %d\n",s);

	}
*/

	return 0; /*len*/
}




