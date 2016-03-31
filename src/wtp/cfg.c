#include <errno.h>
#include <stdio.h>

#include "jsmn.h"
#include "wtp.h"
#include "cw/bstr.h"

#include "cw/mavl.h"
#include "cw/format.h"
#include "cw/file.h"
#include "cw/sock.h"
#include "cw/item.h"
#include "cw/action.h"
#include "cw/mbag.h"
#include "cw/capwap_items.h"
#include "cw/radio.h"
#include "cw/conn.h"
#include "cw/bstr.h"




static int skip(jsmntok_t * t)
{
	switch (t->type) {
		case JSMN_OBJECT:
		{
			int e = t->end;
			int n = 1;
			while (e > t->start) {
				t++;
				n++;
			}
			return n;
		}
		default:
			return t->size + 2;

	}
}

void set_cfg(mbag_t mbag, cw_itemdefheap_t defs, const char *id, const char *subid,const char *val)
{
	//printf("Setting: %s/%s: %s\n",id,subid,val);
	const cw_itemdef_t *idef;

	int dyn=0;
	if (!subid) {
		idef = cw_itemdef_get(defs,id,subid);
	}
	else {
		idef = cw_itemdef_get(defs,id,subid);
		if (!idef){
			idef = cw_itemdef_get(defs,id,CW_ITEM_ANY);
			if (idef)
				dyn=1;
		}
	}

	if (!idef) {
		fprintf(stderr,"CFG: No definition for item %s/%s not found\n",id,subid);
		return ;
	}

	mbag_item_t * item;
	if (idef->type->from_str){
		item=idef->type->from_str(val);
		if (dyn){
			item->id=strdup(subid);
			item->dynid=1;
		}
		else{
			if (subid)
				item->id=idef->sub_id;
			else
				item->id=idef->id;
		}

		item->type=idef->type;
	
		
	}
	else{
		fprintf(stderr,"Can't read item '%s' - no from_str method defined\n",id);
		exit(0);
	}
	

	mbag_t wmbag;
	if (!subid) {
		wmbag=mbag;
	}	
	else{
		wmbag=mbag_get_mbag_c(mbag,id,mbag_create);
	}

	//printf("Adding to Mbag %s:%s\n",item->id,val);	
	mavl_replace(wmbag,item);	

}


static int scn_obj(char *js, jsmntok_t * t, 
		mbag_t mbag,
		cw_itemdefheap_t defs, 
		const char *objkey);



static int scn_radios(char *js, jsmntok_t * t)
{
	int i;

	struct conn * conn = get_conn();

	if (t->type != JSMN_OBJECT) {
		fprintf(stderr,"Error reading json cfgp: Not an object\n");
		return 0;
	}

	int j = 1;
	for (i = 0; i < t->size; i++) {

		jsmntok_t * to=t+j;

		*(js + to->end) = 0;
		const char *key = js + to->start;
		*(js + (to + 1)->end) = 0;
		const char * val = js+(to+1)->start;

		//printf("Radio Key: %s Val: %s\n",key,val);

		if ((to+1)->type == JSMN_OBJECT) {
			int rid = atoi(key);
			
			printf("Radio id %d\n",rid);
			mbag_t radio=mbag_i_get_mbag_c(conn->radios,rid,mbag_create);
			scn_obj(js,to+1,radio,conn->actions->radioitems,NULL);	

		}


		
/*
		if ((to+1)->type == JSMN_OBJECT) {
			if (objkey) {
				fprintf(stderr,"Too deep: %s %s\n",objkey,key);

			}
			else{

				if (strcmp(key,"radios")==0){
					printf("Radios found\n");
					
					exit(0);
				}

			}

		}
*/
		j+=skip(to+1);

	}

	return 0;
}

static int scn_obj(char *js, jsmntok_t * t, 
		mbag_t mbag,
		cw_itemdefheap_t defs, 
		const char *objkey)
{
	int i;

	if (t->type != JSMN_OBJECT) {
		fprintf(stderr,"Error reading json cfgp: Not an object\n");
		return 0;
	}

	int j = 1;
	for (i = 0; i < t->size; i++) {

//		j += scn_obj0(js, t + j, defs, mbag,objkey,type);

		jsmntok_t * to=t+j;

		*(js + to->end) = 0;
		const char *key = js + to->start;
		*(js + (to + 1)->end) = 0;
		const char * val = js+(to+1)->start;

		//printf("Key: %s Val: %s\n",key,val);
		

		if ((to+1)->type == JSMN_OBJECT) {
			if (objkey) {
				fprintf(stderr,"Too deep: %s %s\n",objkey,key);

			}
			else{

				/* special case: radios */
				if (strcmp(key,"radios")==0){
					printf("Radios found\n");
					scn_radios(js,to+1);	
				}
				else{
					scn_obj(js,to+1,mbag,defs,key);	
				}
			}

		}
		else{
			if (objkey)
				set_cfg(mbag,defs,objkey,key,val);
			else
				set_cfg(mbag,defs,key,NULL,val);

		}

		j+=skip(to+1);

	}
	return 0;
}


int cfg_json_put_radios(char *dst, const char *name, mbag_item_t * i, int n)
{
	struct conn *conn = get_conn();
	char *d = dst;


	memset(d, '\t', n);
	d += n;
	d += sprintf(d, "\"radios\":{\n");

	MAVLITER_DEFINE(radios, conn->radios);
	const char *comma = "";
	mavliter_foreach(&radios) {
		mbag_item_t *i = mavliter_get(&radios);

		d += sprintf(d, "%s", comma);
		comma = ",\n";
		memset(d, '\t', n + 1);
		d += n + 1;
		d += sprintf(d, "\"%d\":", i->iid);
	}

	d += sprintf(d, "\n");
	memset(d, '\t', n);
	d += n;
	d += sprintf(d, "}");
	return d - dst;
}

/*
int cfg_json_put_acobj(char *dst, const char *name, mbag_item_t * i, int n)
{
	char *d = dst;
	memset(d, '\t', n);
	d += n;
	d += sprintf(d, "\"%s\":{\n", name);

	MAVLITER_DEFINE(it, i->data);
	char *comma = "";
	mavliter_foreach(&it) {
		cw_acprio_t *acprio = mavliter_get(&it);
		d += sprintf(d, "%s", comma);
		comma = ",\n";
		memset(d, '\t', n + 1);
		d += n + 1;
		d += sprintf(d, "\"%s\":\"%d\"", acprio->name, acprio->prio);

	}
	d += sprintf(d, "\n");
	memset(d, '\t', n);
	d += n;
	d += sprintf(d, "}");




//      d+=mbag_tojson(d,i->data,board_data_cfg,n);
	return d - dst;

}
*/


int mbag_tojson(char *dst, mbag_t m, cw_itemdef_t *defs, int n)
{
	char *d;
	d = dst;

	d += sprintf(d, "%s", "{\n");

	MAVLITER_DEFINE(it, m);

	const char *delim = "";
	mavliter_foreach(&it) {

		mbag_item_t *i = mavliter_get(&it);
		d += sprintf(d, "%s", delim);
		delim = ",\n";
//
		memset(d,'\t',n+1);
		d+=n+1;
		d+=sprintf(d,"\"%s\":",i->id);
//		d+=sprintf(d,"\"%s(%s)\":",i->id,i->type->name);

		if (i->type==MBAG_MBAG){
			d+=mbag_tojson(d,i->data,defs,n+1);
		}
		else{
			d+=sprintf(d,"\"");
			if (i->type->to_str){
				d+=i->type->to_str(i,d);
			}
			d+=sprintf(d,"\"");
		}
	}
	if (n==0){
		struct conn * conn;
		conn = get_conn();
		MAVLITER_DEFINE(ir,conn->radios);

		memset(d, '\t', n);
		d += n;
		d += sprintf(d, "%s", delim);

		d+=sprintf(d,"\t\"radios\":{\n");

		char *delim="";
		mavliter_foreach(&ir){
			memset(d, '\t', n);
			d += n;
			d += sprintf(d, "%s", delim);
			delim = ",\n";

			mbag_item_t *radio = mavliter_get(&ir);
			d+=sprintf(d,"\t\t\"%d\":",radio->iid);	
			d+=mbag_tojson(d,radio->data,defs,n+2);
			

		}
		d += sprintf(d, "\n\t}");

		
	}


	d += sprintf(d, "\n");
	memset(d, '\t', n);
	d += n;


	d += sprintf(d, "%s", "}");

	return d - dst;
}


int cfg_to_json()
{
	struct conn *conn = get_conn();
	char dst[4096];
//	mbag_set_byte(conn->config, CW_ITEM_RADIOS, 1);

	int n = mbag_tojson(dst, conn->config, NULL, 0);
//printf("DST: %s\n",dst);
//exit(0);

	cw_save_file("cfg.json", dst, n);
	return 1;
}




int cfg_from_json(struct conn *conn)
{
	size_t size;
	char *jstr = cw_load_file("cfg.json", &size);
	if (!jstr) {
		fprintf(stderr, "Can't load cfg %s: %s\n", "cfg.json", strerror(errno));
		return 0;
	}

	jsmn_parser p;
	jsmntok_t t[1200];
	jsmn_init(&p);

	int rc = jsmn_parse(&p, jstr, size, t, sizeof(t) / sizeof(t[0]));
	if (rc < 0) {
		printf("Parser failed\n");
		return 1;
	}

	printf("Number of items %d\n",conn->actions->items->count);

	scn_obj(jstr, t, conn->config, conn->actions->items,NULL);

	return 1;
}
