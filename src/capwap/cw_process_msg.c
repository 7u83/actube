
#include <stdint.h>
#include <stdio.h>

#include "conn.h"
#include "capwap.h"
#include "cw_action.h"

int cw_process_msg(struct conn * conn,uint8_t * rawmsg,int len)
{
	struct cw_action as,*af;
	

	uint8_t * msg_ptr = rawmsg+cw_get_hdr_msg_offset(rawmsg);
	
	/* prepare struct for search operation */
	as.elem_id=-1;
	as.capwap_state=conn->capwap_state;
	as.msg_id = cw_get_msg_id(msg_ptr);

	/* Search for state/message combination */
	af = cw_actionlist_get(conn->msgtr,&as);

	if ( !af ){
		printf("Error - Message not found: %d\n",as.msg_id);
		return 0;
	}


	if (af->start){
		af->start(conn,af,rawmsg,len);
	}

	int elems_len = cw_get_msg_elems_len(msg_ptr);
	uint8_t * elems_ptr = cw_get_msg_elems_ptr(msg_ptr);

	

	uint8_t *elem;

	cw_foreach_elem(elem,elems_ptr,elems_len) {

		as.elem_id = cw_get_elem_id(elem);
		int elem_len = cw_get_elem_len(elem);

		af = cw_actionlist_get(conn->msgtr,&as);
		
		if (!af) {
			printf("Wrong Elem: %d, %d\n",as.elem_id,elem_len);
			continue;
		}

		printf("Elem OK: %d, %d\n",as.elem_id,elem_len);

	}


	return 0;

}





