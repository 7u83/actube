
#include "capwap.h"
/** 
 * @file 
 * @brief Implementation of Vendor Specific Payload
 */

/**
 * Default handler for Vendor Specific Payload message elements.
 */ 
int cw_in_vendor_specific_payload(struct conn *conn,struct cw_action * a,uint8_t *data,int len)
{
        cw_action_t as,*af;
	as = *a;

	as.vendor_id = cw_get_dword(data);
	as.elem_id = cw_get_word(data+4);
	printf("Vendor Specific: %d, %d\n",as.vendor_id,as.elem_id);

 	af = cw_actionlist_get(conn->msgtr,&as);



	if (!af) {
		printf("Msg unknown\n");
		return 0;
	}

	printf("Found\n");

	if (af->start) {
		af->start(conn,af,data+6,len-6);
	}

	return 1;
}


