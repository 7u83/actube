
#include "cw/cw.h"
#include "cw/dbg.h"
#include "mod_capwap.h"

/*

static int put_ip(uint8_t *dst void *priv, cw_acip_t * acip void *data)
{
//	uint8_t ** dptr = (uint8_t **)priv; 
//	cw_acip_t *acip=(cw_acip_t*)data;

//	uint8_t * dst = *dptr;

	uint8_t *d = dst+4;

	d+=cw_put_sockaddr(d,&acip->ip);
	d+=cw_put_word(d,acip->index);

	int fam = sock_addrfamily(&acip->ip);
	int elem_id=-1;
	switch (fam) {
		case AF_INET:
			elem_id = CW_ELEM_CAPWAP_CONTROL_IPV4_ADDRESS;
			 break;
		case AF_INET6:
			elem_id = CW_ELEM_CAPWAP_CONTROL_IPV6_ADDRESS;
			break;
		default: 
			cw_log(LOG_ERR,"Unknown adress family %d",fam);
			break;
	}

	if (elem_id ==-1)
		return 0;

	d+=cw_put_elem_hdr(dst,elem_id,d-dst-4);
//		return 0;
//		*dptr = d-4;


	return d-4-dst;

//	return 1;
}

*/


int capwap_out_capwap_control_ip_address(struct cw_ElemHandler * eh, 
		struct cw_ElemHandlerParams * params, uint8_t * dst)
{
	char key[CW_KTV_MAX_KEY_LEN];
	int i;
	int wtps;
	cw_KTV_t * address;
	uint8_t *d;

	d = dst;
	i=0; 
	do {
/*		uint8_t * msg;*/
		int l;

		sprintf(key,"%s/address.%d",eh->key,i);
		address = cw_ktv_get(params->conn->local_cfg,key,CW_TYPE_IPADDRESS);
		sprintf(key,"%s/wtps.%d",eh->key,i);
		wtps = cw_ktv_get_word(params->conn->local_cfg,key,0);
		i++;
		if (address==NULL){
			break;
		}
		
/*		msg = d;*/
		
		l = address->type->len(address);

		switch(eh->id){
			case CW_ELEM_CAPWAP_CONTROL_IPV4_ADDRESS:
				if (l!=4){
					continue;
				}
				break;
			case CW_ELEM_CAPWAP_CONTROL_IPV6_ADDRESS:
				if (l!=16){
					continue;
				}
				break;
			default:
				continue;
		}
		
		l = address->type->put(address,d+4);
		l+=cw_put_word(dst+4+l,wtps);
		l+=cw_put_elem_hdr(d,eh->id,l);
		cw_dbg_elem(DBG_ELEM_OUT,params->conn,params->msgdata->type,eh,d+4,l-4);
		
		d+=l;
	}while(address != NULL);


	return d-dst;
}

/*
int cw_out_capwap_control_ip_addr_list(struct conn *conn,struct cw_action_out *a,uint8_t *dst) 
{
	struct mbag_item * item = mbag_get(conn->local,a->item_id);

	if ( !item ) {
		cw_log(LOG_ERR, "Can't send CAPWAP Local IPv4/IPv6 Address, not found");
		return 0;
	}


	cw_aciplist_t aciplist = (cw_aciplist_t) mbag_item_get_data_ptr(item);


	uint8_t *d = dst;

	MAVLITER_DEFINE(i,aciplist);
	mavliter_foreach(&i){
		struct cw_acip  * acip = mavliter_get(&i);

		d+=put_ip(d,acip);
		
	}


//	cw_aciplist_foreach(aciplist, put_ip, &d);

	mbag_item_release_data_ptr(item,aciplist);	
	return  d-dst;
}

*/
