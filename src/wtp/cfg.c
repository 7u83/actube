#include "capwap/itemstore.h"
#include "capwap/capwap_items.h"
#include "capwap/conn.h"
#include "capwap/bstr.h"

#include <errno.h>
#include <stdio.h>

#include "jsmn.h"
#include "wtp.h"

struct cw_itemdef {
	int item_id;
	const char *cfgname;
	int (*setfun) (struct cw_itemdef *,char *js,jsmntok_t *t);

};
typedef struct cw_itemdef cfg_item_t;

enum {
	CW_ITEMSPACE_DBG,
	CW_ITEMSPACE_WTP
};


int bstr16_local(struct cw_item_def *idef,char *js, jsmntok_t *t)
{
	int item_id = idef=item_id;
	struct conn * conn = get_conn();

	*(js+t->end)=0;
	char *str = js+t->start;
	if (t->type != JSMN_STRING){
		printf("Error: No Str: %s\n",str);
		return 1;
			
	}
//	*(js+t->end)=0;
	printf("Set str: %d %s\n", item_id,str);
	cw_itemstore_set_bstr16n(conn->local,item_id,js+t->start,t->end-t->start);
}


struct cw_itemdef cfg[] = {
	{CW_ITEM_WTP_HARDWARE_VERSION, "hardware_version",bstr16_local}, 
	{CW_ITEM_WTP_SOFTWARE_VERSION, "software_version",bstr16_local}, 
	{CW_ITEM_WTP_BOARD_MODELNO, "modelno",bstr16_local}, 

	{0, 0, 0}
};






struct cw_itemdef * get_cfg(const char *key){
	int i=0;
	for (i=0; cfg[i].item_id; i++){
		if ( !strcmp(key,cfg[i].cfgname ))  {
			return &cfg[i];
		}
	}
	return NULL;
}


int scn_obj(jsmntok_t *t, int i)
{
	


}


static int set_cfg(char *js,jsmntok_t *t){

	*(js+t->end)=0;
	const char * key = js+t->start;

	if ( t->type != JSMN_STRING ) {
		printf("Error - No String: %s\n",key);
		return 0;
	}

	struct cw_itemdef * idef = get_cfg(key);
	if(!idef){
		printf("Error - not found: %s\n",key);
		return 0;
	}	

	if ( !idef->setfun) {
		printf("Error no setfun: %s\n",key);
		
	} 
	else{
		idef->setfun(idef->item_id,js,t+1);
	}

	return 1;	

}

static int read_obj(char *js, jsmntok_t *t ) {
	int i;

	if (t->type!=JSMN_OBJECT){
		return 0;
	}

	if (t->size<3) {
		return 0;
	}

	for (i = 1; i < t->size; i++) {
		i+=set_cfg(js,t+i);
		continue;
				
	}

	return 0;

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

	read_obj(jstr, t);





	int i;







	for (i = 0; cfg[i].item_id != CW_ITEM_NONE; i++) {
		printf("ItemName: %s\n", cfg[i].cfgname);	//.cfgname)

	}

	return 0;
}
