#include "capwap/itemstore.h"
#include "capwap/capwap_items.h"
#include "capwap/conn.h"
#include "capwap/bstr.h"

#include <errno.h>
#include <stdio.h>

#include "jsmn.h"
#include "wtp.h"
#include "capwap/bstr.h"
#include "capwap/radio.h"

struct cw_itemdef {
	int item_id;
	const char *cfgname;
	int (*setfun) (struct cw_itemdef *,char *,jsmntok_t *);
	const char def;

};
typedef struct cw_itemdef cfg_item_t;



enum {
	CW_ITEMSPACE_DBG,
	CW_ITEMSPACE_WTP
};


static int scn_obj(char *js, jsmntok_t *t, int (vcb)(char*js,jsmntok_t*t) ) {
	int i;

	if (t->type!=JSMN_OBJECT){
printf("No object\n");
		return 0;
	}

/*
	if (t->size<3) {
		return 0;
	}

*/
	int j=1;
	for (i = 0; i < t->size; i++) {

		j+=vcb(js,t+j);
		continue;
				
	}

	return 0;

}


static int skip(jsmntok_t *t) 
{
	switch (t->type){
		case JSMN_OBJECT:
		{
			int e=t->end;
			int n=1;
			while (e>t->start) {
				t++;n++;
			}
			return n;
		}
		default:
			return t->size+2;

	}
}





int byte_local(struct cw_itemdef *idef,char *js, jsmntok_t *t)
{
printf("GET BYTE\n");
//	if (t->type != JSMN_STRING){
//		printf("Error: No Str: %s\n",str);
//		return 1;
//	}

	*(js+t->end)=0;
	const char * val = js+t->start;
//	*(js+(t+1)->end)=0;
//	const char * val = js+(t+1)->start;
	struct conn * conn = get_conn();
	
	cw_itemstore_set_byte(conn->local,idef->item_id,atoi(val));

}


int vendorstr_local(struct cw_itemdef *idef,char *js, jsmntok_t *t)
{
	int item_id = idef->item_id;
	struct conn * conn = get_conn();

	*(js+t->end)=0;
	char *str = js+t->start;
	if (t->type != JSMN_ARRAY || t->type!=2){
		printf("Error: No ARRAY or to short: %s\n",str);
		exit(1);
			
	}
//	*(js+t->end)=0;

	char * val = js+(t+1)->start;
	*(js+(t+1)->end)=0;
	printf("Val: %s\n",val);

	uint32_t vendor_id = atoi(val);

	str = (uint8_t*)js+(t+2)->start;

	*((t+2)->end+js)=0;
	bstr16_t v = bstr16cfgstr(str);

printf("Vendor: %d %s\n",vendor_id,str);
	
	cw_itemstore_set_vendorstr(conn->local,item_id,vendor_id,bstr16_data(v),bstr16_len(v));
	free(v);



	return 0;
}




int bstr16_local(struct cw_itemdef *idef,char *js, jsmntok_t *t)
{
	int item_id = idef->item_id;
	struct conn * conn = get_conn();

	*(js+t->end)=0;
	char *str = js+t->start;
	if (t->type != JSMN_STRING){
		printf("Error: No Str: %s\n",str);
		return 1;
			
	}
//	*(js+t->end)=0;
	printf("Set str: %d %s\n", item_id,str);
	cw_itemstore_set_bstr16n(conn->local,item_id,(uint8_t*)js+t->start,t->end-t->start);
	return 0;
}

int bstr_local(struct cw_itemdef *idef,char *js, jsmntok_t *t)
{
	int item_id = idef->item_id;
	struct conn * conn = get_conn();

	*(js+t->end)=0;
	char *str = js+t->start;
	if (t->type != JSMN_STRING){
		printf("Error: No Str: %s\n",str);
		return 1;
			
	}
//	*(js+t->end)=0;
	printf("Set str: %d %s\n", item_id,str);

	bstr16_t v = bstr16cfgstr(str);

	cw_itemstore_set_bstrn(conn->local,item_id,bstr16_data(v),bstr16_len(v)); //(uint8_t*)js+t->start,t->end-t->start);
	free (v);
	return 0;
}




int wtp_board_data_local(struct cw_itemdef *idef,char *js, jsmntok_t *t);





struct cw_itemdef general_cfg[] = {
	{CW_ITEM_AC_NAME, "ac_name",bstr16_local}, 
	{CW_ITEM_WTP_NAME, "wtp_name",bstr16_local}, 
	{CW_ITEM_WTP_HARDWARE_VERSION, "hardware_version",vendorstr_local}, 
	{CW_ITEM_WTP_SOFTWARE_VERSION, "software_version",vendorstr_local}, 
	{CW_ITEM_WTP_BOARD_MODELNO, "modelno",bstr16_local}, 
	{CW_ITEM_WTP_BOARD_DATA,"wtp_board_data",wtp_board_data_local},
	{CW_ITEM_WTP_FRAME_TUNNEL_MODE,"frame_tunnel_mode",byte_local},
	{CW_ITEM_WTP_MAC_TYPE,"mac_type",byte_local},
	{CW_ITEM_LOCATION_DATA,"location_data",bstr16_local},
	{CW_ITEM_WTP_GROUP_NAME,"group_name",bstr16_local},
	{CW_RADIO_BSSID,"bssid",bstr_local},

	{0, 0, 0}
};



struct cw_itemdef board_data_cfg[] = {
	{CW_ITEM_WTP_BOARD_MODELNO, "model_no",NULL}, 
	{CW_ITEM_WTP_BOARD_SERIALNO, "serial_no",NULL}, 
	{CW_ITEM_WTP_BOARD_VENDOR, "vendor_id",NULL},

	{0, 0, 0}
};







struct cw_itemdef * get_cfg(struct cw_itemdef *cfg,const char *key){
	int i=0;
	for (i=0; cfg[i].item_id; i++){
		if ( !strcmp(key,cfg[i].cfgname ))  {
			return &cfg[i];
		}
	}
	return NULL;
}

static int wtp_board_data_cb(char *js,jsmntok_t *t)
{
printf("VB BOARD DATA!!!\n");
	struct conn * conn = get_conn();
	cw_itemstore_t bd = cw_itemstore_get_avltree(conn->local,CW_ITEM_WTP_BOARD_DATA);
	if (!bd){
		bd = cw_itemstore_create();
		if (!bd){
			return skip(t+1);
		}
		cw_itemstore_set_avltree(conn->local,CW_ITEM_WTP_BOARD_DATA,bd);
	}
	



	*(js+t->end)=0;
	const char * key = js+t->start;
	*(js+(t+1)->end)=0;
	const char * val = js+(t+1)->start;
	

	struct conn *get_conn();
	struct cw_itemdef * idef = get_cfg(board_data_cfg,key);

	if (!idef){
		return skip(t+1);
	}

	if (idef->item_id == CW_ITEM_WTP_BOARD_VENDOR){
		cw_itemstore_set_dword(bd,CW_ITEM_WTP_BOARD_VENDOR,atoi(val));
		
	}
	else{
		bstr16_t v = bstr16cfgstr(val);
		cw_itemstore_set_bstr16n(bd,idef->item_id,bstr16_data(v),bstr16_len(v));
		free(v);

	}



	return skip(t+1);

}

int wtp_board_data_local(struct cw_itemdef *idef,char *js, jsmntok_t *t)
{
printf("Local board data\n");

	if ( t->type != JSMN_OBJECT ) {
		printf("Error: wtp_board_data is no object\n");
		printf("Size: %d\n",t->size);
		return 0;
	}
printf("Scanni\n");
	scn_obj(js, t,wtp_board_data_cb);
	return skip(t+1);
}



static int set_cfg(char *js,jsmntok_t *t){

	*(js+t->end)=0;
	const char * key = js+t->start;
	*(js+(t+1)->end)=0;
	//const char * val = js+(t+1)->start;



	struct cw_itemdef * idef = get_cfg(general_cfg,key);

//	printf("Key: %s\n",key);
//	printf("Val: %s\n",val);

	if(!idef)
		return skip(t+1);

	if ( !idef->setfun) {
		printf("Error no setfun: %s\n",key);
		
	} 
	else{
		idef->setfun(idef,js,t+1);
	}

	return skip(t+1);

}




int setup_conf(struct conn *conn)
{
	FILE * infile = fopen("cfg.json","rb");
	if ( !infile ){
		perror("Can't open cfg.json");
		return 0;
	}

	fseek(infile,0,SEEK_END);
	int size = ftell(infile);
	

	char *jstr = malloc(size);
	if ( jstr==NULL){
		perror("Can't allocate memory");
		return 0;

	}

	fseek(infile,0,SEEK_SET);
	fread(jstr,1,size,infile);

	jsmn_parser p;
	jsmntok_t t[1200];
	jsmn_init(&p);

	int rc = jsmn_parse(&p,jstr,size, t,sizeof(t)/sizeof(t[0]));
	if (rc<0) {
		printf("Parser failed\n");
	}

	scn_obj(jstr, t,set_cfg);



void dbg_istore_dmp(cw_itemstore_t s);

dbg_istore_dmp(conn->local);




/*

	for (i = 0; cfg[i].item_id != CW_ITEM_NONE; i++) {
		printf("ItemName: %s\n", cfg[i].cfgname);	//.cfgname)

	}
*/


	return 0;
}
