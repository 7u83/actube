
#include "cw.h"
#include "capwap_items.h"
#include "dbg.h"
#include "mbag.h"

/*
static int read_subelem_cisco(struct ac_info* acinfo,int subtype,uint8_t * elem, int len)
{
	switch (subtype) {
		case 0:
			bstr_replace(&acinfo->hardware_version,bstr_create(elem,len));
			break;
		case 1: 
			bstr_replace(&acinfo->software_version,bstr_create(elem,len));
			break;

		default:
			//printf("What? %d\n",subtype);
			break;

	}
	
	return 1;
}
*/

/*
static int read_subelem(struct ac_info* acinfo,int subtype,uint8_t *elem, int len)
{
	switch (subtype){
		case 0:
		case 4:
			bstr_replace(&acinfo->hardware_version,bstr_create(elem,len));
			break;
		case 1:
		case 5:
			bstr_replace(&acinfo->software_version,bstr_create(elem,len));
			break;
	}

	return 1;
}
*/








static int read_subeelms(struct conn *conn,struct cw_action_in * a,uint8_t *data,int len,struct sockaddr *from)
{
	int sub=12;
	//int sublen;


	while (sub<len){
		if (len-sub<8)
			return 0;


		uint32_t vendor_id = cw_get_dword(data+sub);
		int sublen = cw_get_word(data+sub+6);
		int subtype = cw_get_word(data+sub+4);
printf("substart : %d\n",sub);

		bstrv_t vstr=NULL;
		switch (subtype){
			case 0:
			case 4:
				/* hardware version */
				vstr = mbag_set_bstrv(conn->incomming,CW_ITEM_AC_HARDWARE_VERSION,
					vendor_id,data+sub+8,sublen);
				break;
			case 1:
			case 5:
				/* software version */
				vstr = mbag_set_bstrv(conn->incomming,CW_ITEM_AC_SOFTWARE_VERSION,
					vendor_id,data+sub+8,sublen);
				break;
		}

		cw_dbg_version_subelem(DBG_SUBELEM,"AC Descriptor", subtype, vstr);


		if (sub+sublen>len)
			return -1;

		sub+=sublen+8;
	}

	return 1;	
}






int cw_in_ac_descriptor(struct conn *conn,struct cw_action_in * a,uint8_t *data,int len,struct sockaddr *from)
{
	struct cw_ac_status *status = malloc(sizeof(struct cw_ac_status));
	if (!status)
		return 0;

	status->stations = cw_get_word(data);
	status->limit = cw_get_word(data+2);
	status->active_wtps=cw_get_word(data+4);
	status->max_wtps=cw_get_word(data+6);
	status->security=cw_get_byte(data+8);
	status->rmac_field=cw_get_byte(data+9);
	status->dtls_policy=cw_get_byte(data+11);

	cw_dbg(DBG_SUBELEM,"AC Desriptor: WTPs:%d/%d, Stations:%d/%d, Security:%d, Rmac:%d, DTLS-Policy:%d",
			status->active_wtps,status->max_wtps,
			status->stations,status->limit,
			status->security,
			status->rmac_field,
			status->dtls_policy);


	mbag_set_ptr(conn->incomming,CW_ITEM_AC_STATUS,status);

	read_subeelms(conn,a,data,len,from);
	
	return 1;
}

