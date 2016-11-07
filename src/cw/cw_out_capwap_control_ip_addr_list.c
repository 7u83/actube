
#include "log.h"

#include "cw.h"
#include "aciplist.h"
#include "sock.h"


static int put_ip(uint8_t *dst /*void *priv*/, cw_acip_t * acip /*void *data*/)
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

