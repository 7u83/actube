
#include <string.h>


#include "capwap.h"
#include "capwap_cisco.h"
#include "conn.h"

void cwsend_join_response(struct conn * conn,int seqnum, int rc, struct radioinfo * radioinfo, struct ac_info * acinfo, struct wtpinfo * wtpinfo)
{
	struct cwmsg * cwmsg = &conn->resp_msg;	
	cwmsg_init(cwmsg,conn->resp_buffer,CWMSG_JOIN_RESPONSE,seqnum,NULL);
	cwmsg->capwap_mode=conn->capwap_mode;

	/* mandatory messagesg elements */
	cwmsg_addelem_result_code(cwmsg,rc);
	cwmsg_addelem_ac_descriptor(cwmsg,acinfo);
	cwmsg_addelem(cwmsg,CWMSGELEM_AC_NAME,acinfo->ac_name,strlen((char*)acinfo->ac_name));
	cwmsg_addelem(cwmsg,CWMSGELEM_ECN_SUPPORT,&acinfo->ecn_support,sizeof(uint8_t));
	cwmsg_addelem_ctrl_ip_addrs(cwmsg,acinfo);
//	cwmsg_addelem_cw_local_ip_addr(cwmsg,conn);


	/* */
//	cwmsg_addelem_image_identifier(cwmsg,CW_VENDOR_ID_CISCO,(uint8_t*)"/tobias",strlen("/tobias"));


	uint8_t buffer[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	buffer[0]=0;	/* Mwar Type */
	buffer[1]=1;	/* h/w version */
	buffer[2]=2;	/* h/w Release */
	buffer[3]=0;	/* h/w Maint */
	buffer[4]=3;	/* h/w Build */

	buffer[5]=5;	/* s/w version */
	buffer[6]=0;	/* s/w Release */
	buffer[7]=19;	/* s/w Maint */
	buffer[8]=2;	/* s/w Build */

	*((uint16_t*)(buffer+9))=htons(13); /* Active MS */
	*((uint16_t*)(buffer+9+2))=htons(23); /* Supported MS */
	*((uint16_t*)(buffer+9+4))=htons(5); /* Active RAD's */
	*((uint16_t*)(buffer+9+6))=htons(15); /* Supported RAD's */
//	cwmsg_addelem_vendor_specific_payload(cwmsg,CW_VENDOR_ID_CISCO, CWVENDOR_CISCO_MWAR, buffer,34);

	
	conn_send_response(conn,cwmsg,seqnum);

}



