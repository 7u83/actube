#include <stdlib.h>

#include "mavl.h"
#include "cw/sock.h"
#include "cw/timer.h"
#include "cw/cw.h"
#include "cw/format.h"
#include "cw/conn.h"

#include "cw/log.h"
#include "cw/dbg.h"
#include "cw/file.h"
#include "cw/dot11.h"

#include "wtplist.h"
#include "dataman.h"


struct mavl *dataman_list;
pthread_mutex_t dataman_list_mutex;

static int cmp(const void *d1, const void *d2)
{
	struct cw_Conn *nc1 = ((struct dataman *) d1)->nc;
	struct cw_Conn *nc2 = ((struct dataman *) d1)->nc;

	int r = nc1->sock - nc2->sock;
	if (r != 0)
		return r;

	return sock_cmpaddr((struct sockaddr *) &nc1->addr,
			    (struct sockaddr *) &nc2->addr, 1);

}


int dataman_list_init()
{
	dataman_list = mavl_create_ptr(cmp, NULL);
	if (!dataman_list)
		return 0;

        if (pthread_mutex_init(&dataman_list_mutex,NULL)){
		mavl_destroy(dataman_list);
		return 0;
	}

	return 1;
}



void dataman_destroy(struct dataman *dm)
{
	if (!dm)
		return;
	conn_destroy(dm->nc);
}

struct dataman *dataman_create(int sock, struct sockaddr *addr)
{
	struct dataman *dm = malloc(sizeof(struct dataman));
	if (!dm)
		return NULL;
	memset(dm,0,sizeof(struct dataman));
	dm->nc = cw_conn_create(sock, addr, 100);
	return dm;
}


struct dataman *dataman_list_get(int sock, struct sockaddr *addr)
{
	struct cw_Conn search_nc;
	struct dataman search_dm;

	search_nc.sock = sock;
	sock_copyaddr (&search_nc.addr,addr);

	search_dm.nc = &search_nc;

	struct dataman *dm = mavl_get(dataman_list, &search_dm);

	cw_dbg(DBG_X,"Getting dataman %p",dm); 
	return dm;
}

struct dataman *dataman_list_add(struct dataman *dm)
{

	cw_dbg(DBG_X,"Adding dataman %p\n",dm);
	return mavl_insert(dataman_list, dm,NULL);
}



int dataman_process_keep_alive(struct cw_Conn *nc, uint8_t *rawmsg, int len)
{
	struct dataman * dm = (struct dataman *)(nc->data);

	uint8_t *msgptr = rawmsg + cw_get_hdr_msg_offset(rawmsg);


	int elems_len = cw_get_datamsg_elems_len(msgptr);
	uint8_t * elems_ptr = cw_get_datamsg_elems_ptr(msgptr);
	uint8_t * elem;

	cw_foreach_elem(elem, elems_ptr, elems_len) {

		if (cw_get_elem_id(elem) == CAPWAP_ELEM_SESSION_ID){
			uint8_t sessid[64];
			memset(sessid,0,16);

			int sessid_len = cw_get_elem_len(elem);

			printf("Sess id len = %d\n",sessid_len);

			((uint16_t*)sessid)[0]=sessid_len;
			memcpy(bstr16_data(sessid),cw_get_elem_data(elem),sessid_len);
			
			stop();
			struct wtpman * wtpman =NULL;
//			struct wtpman * wtpman = wtplist_get_by_session_id(sessid);
			if (wtpman){
				if (!dm->wtpman)
					dm->wtpman=wtpman;

				uint8_t buffer[128];
				uint8_t * dl = cw_init_data_keep_alive_msg(buffer,NULL);
				uint8_t * d=dl+2;

				int l = cw_put_elem_session_id(d,bstr16_data(sessid),sessid_len);
				cw_put_word(dl,l);

				int total_len = dl-buffer + l+2;
				printf("len len %d\n",l);
				printf("Total len = %d\n",total_len);

				cw_send_msg(nc,buffer,total_len);
				return len;
					





				printf("Found!i yes I vae found it\n");
					
			}



			int l = cw_get_elem_len(elem);

			printf("ElemID: %d, len = %d\n", cw_get_elem_id(elem),l);
			char cb[64];
			format_hex(cb,sessid,16);
			printf("Session ID: %s",cb);




		}



	}

	errno=EAGAIN;
	return -1;	
}

int dataman_process_message0(struct cw_Conn *nc, uint8_t * rawmsg, int len,
			struct sockaddr *from)
{

	static int c=0;
	char fn[100];
	sprintf(fn,"wificap-%03d",c++);
///	cw_save_file(fn,(char*)rawmsg,len);
///	cw_dbg(DBG_X,"saving %d bytes",len);

	uint8_t * dot11frame = rawmsg + cw_get_hdr_msg_offset(rawmsg);

	cw_dbg(DBG_X,"802.11 - %s",dot11_get_frame_name(dot11frame));
	
	/* The very first data message MUST be a keep-alive message */
	if (!cw_get_hdr_flag_k(rawmsg)){

		cw_dbg(DBG_X,"No K Flag founde");
		errno = EAGAIN;
		return -1;
	}

	cw_dbg(DBG_X, "Goto Keep Alive Pack");
	return dataman_process_keep_alive(nc,rawmsg,len);
}

int dataman_process_message(struct cw_Conn *nc, uint8_t * rawmsg, int len,
			struct sockaddr *from)
{
	if (cw_get_hdr_flag_k(rawmsg)){
		return dataman_process_keep_alive(nc,rawmsg,len);
	}
	static int c=0;

	char fn[100];
	sprintf(fn,"wificap-%03d",c++);
	cw_save_file(fn,(char*)rawmsg,len);


	cw_dbg(DBG_X,"There was someting else than dataman");

	return 1;

}

void dataman_run(struct dataman *dm)
{
	time_t timer = cw_timer_start(2);

	dm->nc->process_packet=conn_process_packet;
	dm->nc->process_message=dataman_process_message0;
	dm->nc->data = dm;


	while (1){
		time_t timer = cw_timer_start(2);
		while (!cw_timer_timeout(timer)){
			cw_read_messages(dm->nc);
		}
	}

	

	while (!cw_timer_timeout(timer)){
		cw_read_messages(dm->nc);
	}

	if (!dm->wtpman){
		cw_log(LOG_ERR,"Data session not associated");
		dataman_destroy(dm);
		return;
	}

	cw_dbg(DBG_X,"Data channel established");
	dm->nc->process_message=dataman_process_message;

	while (1){
		time_t timer = cw_timer_start(2);
		while (!cw_timer_timeout(timer)){
			cw_read_messages(dm->nc);
		}
	}

	
	
}

void dataman_start(struct dataman * dm)
{
	pthread_create(&dm->thread, NULL, (void *) dataman_run,
			       (void *) dm);

}
