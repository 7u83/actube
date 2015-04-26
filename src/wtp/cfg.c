#include "capwap/mbag.h"
#include "capwap/capwap_items.h"
#include "capwap/conn.h"
#include "capwap/bstr.h"

#include <errno.h>
#include <stdio.h>

#include "jsmn.h"
#include "wtp.h"
#include "capwap/bstr.h"
#include "capwap/radio.h"

#include "capwap/mavl.h"
#include "capwap/format.h"

#include "capwap/file.h"
#include "capwap/aciplist.h"
#include "capwap/acpriolist.h"
#include "capwap/sock.h"


/* json putters */
int cfg_json_put_bstr16(char *dst, const char *name, mbag_item_t * i, int n);
int cfg_json_put_vendorstr(char *dst, const char *name, mbag_item_t * i, int n);
int cfg_json_put_dword(char *dst, const char *name, mbag_item_t * i, int n);
int cfg_json_put_word(char *dst, const char *name, mbag_item_t * i, int n);
int cfg_json_put_ac_ip_list(char *dst, const char *name, mbag_item_t * i, int n);
int cfg_json_put_acobj(char *dst, const char *name, mbag_item_t * i, int n);
int cfg_json_put_byte(char *dst, const char *name, mbag_item_t * i, int n);
int cfg_json_put_radios(char *dst, const char *name, mbag_item_t * i, int n);


//static int scn_obj(char *js, jsmntok_t *t, int (vcb)(char*js,jsmntok_t*t,struct mbag_itemdef *defs,mbag_t mbag), struct mbag_itemdef *defs,mbag_t mbag);


struct mbag_itemdef {
	int item_id;
	const char *cfgname;
	int (*setfun) (struct mbag_itemdef *, char *, jsmntok_t *, mbag_t mbag);
	int (*tojsonfun) (char *dst, const char *name, mbag_item_t * i, int n);

};
typedef struct mbag_itemdef cfg_item_t;

int cfg_json_get_dword(struct mbag_itemdef *idef, char *js, jsmntok_t * t, mbag_t mbag);
int cfg_json_get_word(struct mbag_itemdef *idef, char *js, jsmntok_t * t, mbag_t mbag);
int cfg_json_get_byte(struct mbag_itemdef *idef, char *js, jsmntok_t * t, mbag_t mbag);
int cfg_json_get_ac_name_with_priority(struct mbag_itemdef *idef, char *js, jsmntok_t * t,
				       mbag_t mbag);



enum {
	CW_ITEMSPACE_DBG,
	CW_ITEMSPACE_WTP
};


static int scn_obj(char *js, jsmntok_t * t,
		   int (vcb) (char *js, jsmntok_t * t, struct mbag_itemdef * defs,
			      mbag_t mbag), struct mbag_itemdef *defs, mbag_t mbag)
{
	int i;

	if (t->type != JSMN_OBJECT) {
		printf("No object\n");
		return 0;
	}

/*
	if (t->size<3) {
		return 0;
	}

*/
	int j = 1;
	for (i = 0; i < t->size; i++) {

		j += vcb(js, t + j, defs, mbag);
		continue;

	}

	return 0;

}


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

int cfg_json_put_ac_ip_list(char *dst, const char *name, mbag_item_t * i, int n)
{

	char *d = dst;

	cw_aciplist_t aciplist = i->data;

	MAVLITER_DEFINE(it, aciplist);

	memset(d, '\t', n);
	d += n;
	d += sprintf(d, "\"%s\":", name);

	d += sprintf(d, "[\n");
	const char *comma = "";
	mavliter_foreach(&it) {
		cw_acip_t *acip = mavliter_get(&it);

		d += sprintf(d, "%s", comma);
		memset(d, '\t', n + 1);
		d += n + 1;

		d += sprintf(d, "\"%s\"", sock_addr2str(&acip->ip));
		comma = ",\n";


	}
	d += sprintf(d, "\n");
	memset(d, '\t', n);
	d += n;

	d += sprintf(d, "]");

	return d - dst;
}

int cfg_json_get_ac_ip_list(struct mbag_itemdef *idef, char *js, jsmntok_t * t,
			    mbag_t mbag)
{

//	printf("Get AC IP List len = %d\n", t->size);
	if (t->type != JSMN_ARRAY) {
		printf("ac_ip_list is not a list\n");
		exit(0);

	}
	int size = t->size;
	t++;

	int i;

	for (i = 0; i < size; i++, t++) {
		js[t->end] = 0;

		int rc;

		cw_acip_t *acip;
		acip = malloc(sizeof(cw_acip_t));
		if (!acip)
			continue;

		/* convert IPv4 adddress */
		rc = inet_pton(AF_INET, js + t->start,
			       &(((struct sockaddr_in *) (&acip->ip))->sin_addr));
		((struct sockaddr *) (&acip->ip))->sa_family = AF_INET;
		if (!rc) {
			/* If it's not an IPv4 adress, try IPv6 */
			rc = inet_pton(AF_INET6, js + t->start,
				       &(((struct sockaddr_in6 *) (&acip->ip))->
					 sin6_addr));
			((struct sockaddr *) (&acip->ip))->sa_family = AF_INET6;
		}

		if (!rc) {

			printf("Not an IP adress: %s\n", js + t->start);
			exit(0);
		}

		cw_aciplist_t aciplist =
		    mbag_get_mavl(mbag, CW_ITEM_AC_IP_LIST, cw_aciplist_create);
		mavl_replace(aciplist, acip);


	}


	return 0;
}


/*
int byte_local(struct mbag_itemdef *idef, char *js, jsmntok_t * t)
{
	printf("GET BYTE\n");
//      if (t->type != JSMN_STRING){
//              printf("Error: No Str: %s\n",str);
//              return 1;
//      }

	*(js + t->end) = 0;
	const char *val = js + t->start;
//      *(js+(t+1)->end)=0;
//      const char * val = js+(t+1)->start;
	struct conn *conn = get_conn();

	mbag_set_byte(conn->local, idef->item_id, atoi(val));
	return 0;
}
*/

int cfg_json_get_vendorstr(struct mbag_itemdef *idef, char *js, jsmntok_t * t,
			   mbag_t mbag)
{
	int item_id = idef->item_id;

	*(js + t->end) = 0;
	char *str = js + t->start;
	if (t->type != JSMN_ARRAY || t->type != 2) {
		return 0;
	}

	char *val = js + (t + 1)->start;
	*(js + (t + 1)->end) = 0;

	uint32_t vendor_id = atoi(val);

	str = (char *) (js + (t + 2)->start);

	*((t + 2)->end + js) = 0;
	bstr16_t v = bstr16cfgstr(str);

	mbag_set_vendorstr(mbag, item_id, vendor_id, bstr16_data(v), bstr16_len(v));
	free(v);
	return 0;
}


int cfg_json_get_bstr16(struct mbag_itemdef *idef, char *js, jsmntok_t * t, mbag_t mbag)
{

	struct conn *conn = get_conn();

	int item_id = idef->item_id;
	*(js + t->end) = 0;
//      char *str = js+t->start;
	if (t->type != JSMN_STRING) {
		return 0;
	}
	*(js + t->end) = 0;
	bstr16_t b = bstr16cfgstr(js + t->start);
	mbag_set_bstr16(mbag, item_id, b);
	return 0;
}


int cfg_json_get_dword(struct mbag_itemdef *idef, char *js, jsmntok_t * t, mbag_t mbag)
{

	struct conn *conn = get_conn();

	int item_id = idef->item_id;
	*(js + t->end) = 0;
//      char *str = js+t->start;
	if (t->type != JSMN_STRING) {
		return 0;
	}
	*(js + t->end) = 0;
	uint32_t dw = atoi(js + t->start);
	mbag_set_dword(mbag, item_id, dw);
	return 0;
}

int cfg_json_get_word(struct mbag_itemdef *idef, char *js, jsmntok_t * t, mbag_t mbag)
{

	struct conn *conn = get_conn();

	int item_id = idef->item_id;
	*(js + t->end) = 0;
//      char *str = js+t->start;
	if (t->type != JSMN_STRING) {
		return 0;
	}
	*(js + t->end) = 0;
	uint32_t dw = atoi(js + t->start);
	mbag_set_word(mbag, item_id, dw);
	return 0;
}


int cfg_json_get_byte(struct mbag_itemdef *idef, char *js, jsmntok_t * t, mbag_t mbag)
{

	struct conn *conn = get_conn();

	int item_id = idef->item_id;
	*(js + t->end) = 0;
//      char *str = js+t->start;
	if (t->type != JSMN_STRING) {
		return 0;
	}
	*(js + t->end) = 0;
	uint32_t dw = atoi(js + t->start);
	mbag_set_byte(mbag, item_id, dw);
	return 0;
}




int bstr16_local(struct mbag_itemdef *idef, char *js, jsmntok_t * t)
{
	int item_id = idef->item_id;
	struct conn *conn = get_conn();

	*(js + t->end) = 0;
	char *str = js + t->start;
	if (t->type != JSMN_STRING) {
		printf("Error: No Str: %s\n", str);
		return 1;

	}
//      *(js+t->end)=0;
//	printf("Set str: %d %s\n", item_id, str);
	mbag_set_bstr16n(conn->local, item_id, (uint8_t *) js + t->start,
			 t->end - t->start);
	return 0;
}

int bstr_local(struct mbag_itemdef *idef, char *js, jsmntok_t * t)
{
	int item_id = idef->item_id;
	struct conn *conn = get_conn();

	*(js + t->end) = 0;
	char *str = js + t->start;
	if (t->type != JSMN_STRING) {
		printf("Error: No Str: %s\n", str);
		return 1;

	}
//      *(js+t->end)=0;
//	printf("Set str: %d %s\n", item_id, str);

	bstr16_t v = bstr16cfgstr(str);

	mbag_set_bstrn(conn->local, item_id, bstr16_data(v), bstr16_len(v));	//(uint8_t*)js+t->start,t->end-t->start);
	free(v);
	return 0;
}



int cfg_json_get_ac_ip_list(struct mbag_itemdef *idef, char *js, jsmntok_t * t,
			    mbag_t mbag);

int wtp_board_data_local(struct mbag_itemdef *idef, char *js, jsmntok_t * t, mbag_t mbag);
int cfg_json_put_obj(char *dst, const char *name, mbag_item_t * i, int n);

int cfg_json_get_radios(struct mbag_itemdef *idef, char *js, jsmntok_t * t, mbag_t mbag);




struct mbag_itemdef general_cfg[] = {
	{CW_ITEM_WTP_NAME, "wtp_name", cfg_json_get_bstr16, cfg_json_put_bstr16},
	{CW_ITEM_WTP_GROUP_NAME, "wtp_group_name", cfg_json_get_bstr16,
	 cfg_json_put_bstr16},
	{CW_ITEM_LOCATION_DATA, "location_data", cfg_json_get_bstr16,
	 cfg_json_put_bstr16},
	{CW_ITEM_WTP_HARDWARE_VERSION, "hardware_version", cfg_json_get_vendorstr,
	 cfg_json_put_vendorstr},
	{CW_ITEM_WTP_SOFTWARE_VERSION, "software_version", cfg_json_get_vendorstr,
	 cfg_json_put_vendorstr},
	{CW_ITEM_WTP_BOARD_DATA, "wtp_board_data", wtp_board_data_local,
	 cfg_json_put_obj},
	{CW_RADIO_BSSID, "bssid", cfg_json_get_bstr16, cfg_json_put_bstr16},
	{CW_ITEM_CAPWAP_TIMERS, "capwap_timers", cfg_json_get_word, cfg_json_put_word},
	{CW_ITEM_IDLE_TIMEOUT, "idle_timeout", cfg_json_get_dword, cfg_json_put_dword},
	{CW_ITEM_AC_IP_LIST, "ac_ip_list", cfg_json_get_ac_ip_list,
	 cfg_json_put_ac_ip_list},

	{CW_ITEM_AP_MODE_AND_TYPE, "ap_mode_and_type", cfg_json_get_word,
	 cfg_json_put_word},

	{CW_ITEM_WTP_FRAME_TUNNEL_MODE, "wtp_frame_tunnel_mode", cfg_json_get_byte,
	 cfg_json_put_byte},
	{CW_ITEM_WTP_MAC_TYPE, "wtp_mac_type", cfg_json_get_byte, cfg_json_put_byte},

	{CW_ITEM_RADIOS, "radios", cfg_json_get_radios, cfg_json_put_radios},


	{CW_ITEM_AC_NAME_WITH_PRIORITY, "ac_name_with_priority",
	 cfg_json_get_ac_name_with_priority, cfg_json_put_acobj},

	{CW_ITEM_AC_HASH_VALUE, "ac_hash_value", cfg_json_get_bstr16,
	 cfg_json_put_bstr16},

	{CW_ITEM_CISCO_BOARD_DATA_OPTIONS, "wtp_board_data_options", cfg_json_get_bstr16,
	 cfg_json_put_bstr16},


/*	{CW_ITEM_WTP_FRAME_TUNNEL_MODE,"frame_tunnel_mode",byte_local},
	{CW_ITEM_WTP_MAC_TYPE,"mac_type",byte_local},
	{CW_ITEM_WTP_GROUP_NAME,"group_name",bstr16_local},
	{CW_RADIO_BSSID,"bssid",bstr_local},
*/
	{0, 0, 0}
};



struct mbag_itemdef board_data_cfg[] = {
	{CW_ITEM_WTP_BOARD_MODELNO, "model_no", cfg_json_get_bstr16, cfg_json_put_bstr16},
	{CW_ITEM_WTP_BOARD_SERIALNO, "serial_no", cfg_json_get_bstr16,
	 cfg_json_put_bstr16},
	{CW_ITEM_WTP_BOARD_VENDOR, "vendor_id", cfg_json_get_dword, cfg_json_put_dword},

	{0, 0, 0}
};


struct mbag_itemdef radio_cfg[] = {
	{CW_RADIO_ADMIN_STATE, "admin_state", cfg_json_get_byte, cfg_json_put_byte},
	{CW_RADIO_TYPE, "radio_type", cfg_json_get_dword, cfg_json_put_dword},
	{0,0,0}
	

};


struct mbag_itemdef *get_idef_by_id(struct mbag_itemdef *cfg, uint32_t id)
{
	int i = 0;
	for (i = 0; cfg[i].item_id; i++) {
		if (cfg[i].item_id == id) {
			return &cfg[i];
		}
	}
	return NULL;
}






int cfg_json_put_bstr16(char *dst, const char *name, mbag_item_t * i, int n)
{
	if (i->type != MBAG_BSTR16) {
		return 0;
	}

	char *d = dst;
	memset(d, '\t', n);
	d += n;
	d += sprintf(d, "\"%s\":", name);

	int utf8 = cw_is_utf8(bstr16_data(i->data), bstr16_len(i->data));
	if (utf8) {
		d += sprintf(d, "\"%.*s\"", bstr16_len(i->data), bstr16_data(i->data));
	} else {
		d += sprintf(d, "\".x");
		d += cw_format_hex(d, bstr16_data(i->data), bstr16_len(i->data));
		d += sprintf(d, "\"");
	}
	return d - dst;
}


int cfg_json_put_dword(char *dst, const char *name, mbag_item_t * i, int n)
{
//      if (i->type != MBAG_BSTR16){
//              return 0;
//      }

	char *d = dst;
	memset(d, '\t', n);
	d += n;
	d += sprintf(d, "\"%s\":", name);
	d += sprintf(d, "\"%d\"", i->dword);
	return d - dst;
}

int cfg_json_put_word(char *dst, const char *name, mbag_item_t * i, int n)
{
//      if (i->type != MBAG_BSTR16){
//              return 0;
//      }

	char *d = dst;
	memset(d, '\t', n);
	d += n;
	d += sprintf(d, "\"%s\":", name);
	d += sprintf(d, "\"%d\"", i->word);
	return d - dst;
}

int cfg_json_put_byte(char *dst, const char *name, mbag_item_t * i, int n)
{
//      if (i->type != MBAG_BSTR16){
//              return 0;
//      }

	char *d = dst;
	memset(d, '\t', n);
	d += n;
	d += sprintf(d, "\"%s\":", name);
	d += sprintf(d, "\"%d\"", i->byte);
	return d - dst;
}




int cfg_json_put_vendorstr(char *dst, const char *name, mbag_item_t * i, int n)
{
	if (i->type != MBAG_VENDORSTR) {
		return 0;
	}

	char *d = dst;
	memset(d, '\t', n);
	d += n;
	d += sprintf(d, "\"%s\":", name);
	d += sprintf(d, "[\"%d\",", vendorstr_get_vendor_id(i->data));

	if (cw_is_utf8(vendorstr_data(i->data), vendorstr_len(i->data))) {
		d += sprintf(d, "\"%.*s\"", vendorstr_len(i->data),
			     vendorstr_data(i->data));
	} else {
		d += sprintf(d, "\".x");
		d += cw_format_hex(d, vendorstr_data(i->data), vendorstr_len(i->data));
		d += sprintf(d, "\"");

	}


	d += sprintf(d, "]");

//      d+=sprintf(d,"\"%.*s\",\n",bstr16_len(i->data),bstr16_data(i->data));
	return d - dst;
}

int mbag_tojson(char *dst, mbag_t m, struct mbag_itemdef *defs, int n);

int cfg_json_put_obj(char *dst, const char *name, mbag_item_t * i, int n)
{
	char *d = dst;
	memset(d, '\t', n);
	d += n;
	d += sprintf(d, "\"%s\":", name);
	d += mbag_tojson(d, i->data, board_data_cfg, n);
	return d - dst;

}

int cfg_json_put_radios(char *dst, const char *name, mbag_item_t * i, int n)
{
	struct conn * conn = get_conn();
	char *d = dst;


	memset(d, '\t', n);
	d += n;
	d += sprintf(d, "\"radios\":{\n");
	
	MAVLITER_DEFINE(radios,conn->radios);
	const char * comma="";
	mavliter_foreach(&radios) {
		mbag_item_t *i = mavliter_get(&radios);
		int rid = i->id;
		mbag_t radio = i->data;

		d+=sprintf(d,"%s",comma);
		comma=",\n";
		memset(d, '\t', n+1);
		d += n+1;
		d += sprintf(d, "\"%d\":", i->id);
		
		d += mbag_tojson(d, i->data, radio_cfg, n+1);
		
		



	}

	d+=sprintf(d,"\n");
	memset(d, '\t', n);
	d += n;
	d += sprintf(d, "}");


	return d-dst;		


}


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



int mbag_tojson(char *dst, mbag_t m, struct mbag_itemdef *defs, int n)
{
	char *d;
	d = dst;


//      memset(dst,'\t',n);
//      d+=n;
	d += sprintf(d, "%s", "{\n");

	MAVLITER_DEFINE(it, m);

	const char *delim = "";
	mavliter_foreach(&it) {
		mbag_item_t *i = mavliter_get(&it);

/*
		if (i->type == MBAG_MBAG){
			printf("MBAG DETECTD\n");

			d+=mbag_tojson(d,i->data,n+1);
			continue;

		}

*/
//d+=sprintf(d,"jele .");

		struct mbag_itemdef *idef = get_idef_by_id(defs, i->id);
		if (idef == 0) {
			continue;
		}
		if (!idef->tojsonfun)
			continue;

		d += sprintf(d, "%s", delim);
		delim = ",\n";
		d += idef->tojsonfun(d, idef->cfgname, i, n + 1);

	}
	d += sprintf(d, "\n");
	memset(d, '\t', n);
	d += n;

	d += sprintf(d, "%s", "}");

	return d - dst;
}

int cfg_json_save()
{
	struct conn *conn = get_conn();
	char dst[4096];
	mbag_set_byte(conn->config,CW_ITEM_RADIOS,1);	

	int n = mbag_tojson(dst, conn->config, general_cfg, 0);


////printf("DST:%s\n",dst);
//exit(0);
	cw_save_file("cfg.json", dst, n);
	return 1;
}

struct mbag_itemdef *get_cfg(struct mbag_itemdef *cfg, const char *key)
{
	int i = 0;
	for (i = 0; cfg[i].item_id; i++) {
		if (!strcmp(key, cfg[i].cfgname)) {
			return &cfg[i];
		}
	}
	return NULL;
}

static int wtp_board_data_cb(char *js, jsmntok_t * t, mbag_t mbag)
{
	struct conn *conn = get_conn();
	mbag_t bd = mbag_get_mbag(conn->config, CW_ITEM_WTP_BOARD_DATA, NULL);
	if (!bd) {
		bd = mbag_create();
		if (!bd) {
			return skip(t + 1);
		}
		mbag_set_mbag(mbag, CW_ITEM_WTP_BOARD_DATA, bd);
	}




	*(js + t->end) = 0;
	const char *key = js + t->start;
	*(js + (t + 1)->end) = 0;
	const char *val = js + (t + 1)->start;


	struct conn *get_conn();
	struct mbag_itemdef *idef = get_cfg(board_data_cfg, key);

	if (!idef) {
		return skip(t + 1);
	}

	if (idef->item_id == CW_ITEM_WTP_BOARD_VENDOR) {
		mbag_set_dword(bd, CW_ITEM_WTP_BOARD_VENDOR, atoi(val));

	} else {
		bstr16_t v = bstr16cfgstr(val);
		mbag_set_bstr16n(bd, idef->item_id, bstr16_data(v), bstr16_len(v));
		free(v);

	}



	return skip(t + 1);

}

static int set_cfg(char *js, jsmntok_t * t, struct mbag_itemdef *defs, mbag_t mbag);



static int cfg_json_get_ac_name_with_priority_cb(char *js, jsmntok_t * t,
						 struct mbag_itemdef *defs, mbag_t mbag)
{

	*(js + t->end) = 0;
	const char *key = js + t->start;
	*(js + (t + 1)->end) = 0;
	const char *val = js + (t + 1)->start;


	if ((t + 1)->type != JSMN_STRING) {
		printf("Errro no string acname with index");
		return skip(t + 1);

	}

	cw_acprio_t *ac = malloc(sizeof(cw_acprio_t));
	if (!ac) {
		return skip(t + 1);
	}

	ac->name = strdup(key);
	ac->prio = atoi(val);

	//mbag_item_t * i = mbag_item_create2(mbag,ac->name);
//      i->type = MBAG_ACPRIO;



//      printf("ACNAMEKEY: %s\n",key);
//	printf("Key: %s\n", key);
//	printf("Val: %s\n", val);

	mavl_replace(mbag, ac);
	return skip(t + 1);
}



int cfg_json_get_ac_name_with_priority(struct mbag_itemdef *idef, char *js, jsmntok_t * t,
				       mbag_t mbag)
{

	if (t->type != JSMN_OBJECT) {
		printf("Error: AC_NAME_WITH PRIORITY should be an object\n");
		printf("Size: %d\n", t->size);
		return 0;
	}

	struct conn *conn = get_conn();
	cw_acpriolist_t al =
	    mbag_get_mavl(conn->config, CW_ITEM_AC_NAME_WITH_PRIORITY,
			  cw_acpriolist_create);
	if (!al)
		return skip(t + 1);

	scn_obj(js, t, cfg_json_get_ac_name_with_priority_cb, NULL, al);

	return skip(t + 1);
}



static int cfg_json_get_radios_cb(char *js, jsmntok_t * t,
						 struct mbag_itemdef *defs, mbag_t mbag)
{

	*(js + t->end) = 0;
	const char *key = js + t->start;
	*(js + (t + 1)->end) = 0;
	const char *val = js + (t + 1)->start;

//	printf("Key: %s\n",key);
//	printf("Val: %s\n",val);

	

	struct conn * conn = get_conn();


	mbag_t radio = mbag_get_mbag(conn->radios,atoi(key),NULL);
	if ( !radio ){
		printf("Radio %d not found\n",atoi(key));
		return skip(t+1);

	}

	scn_obj(js, t+1, set_cfg, radio_cfg, radio);

	return skip(t+1);	
}


int cfg_json_get_radios(struct mbag_itemdef *idef, char *js, jsmntok_t * t, mbag_t mbag)
{

	if (t->type != JSMN_OBJECT) {
		printf("Error: Radios is no object\n");
		printf("Size: %d\n", t->size);
		exit(0);
		return 0;
	}

	struct conn *conn = get_conn();
	cw_acpriolist_t al =
	    mbag_get_mavl(conn->config, CW_ITEM_AC_NAME_WITH_PRIORITY,
			  cw_acpriolist_create);
	if (!al)
		return skip(t + 1);

	scn_obj(js, t, cfg_json_get_radios_cb, NULL, NULL);

	return skip(t + 1);

}








int wtp_board_data_local(struct mbag_itemdef *idef, char *js, jsmntok_t * t, mbag_t mbag)
{

	if (t->type != JSMN_OBJECT) {
		printf("Error: wtp_board_data is no object\n");
		printf("Size: %d\n", t->size);
		return 0;
	}

	struct conn *conn = get_conn();
	mbag_t bd = mbag_get_mbag(conn->config, CW_ITEM_WTP_BOARD_DATA, NULL);
	if (!bd) {
		bd = mbag_create();
		if (!bd) {
			return skip(t + 1);
		}
		mbag_set_mbag(mbag, CW_ITEM_WTP_BOARD_DATA, bd);
	}

	scn_obj(js, t, set_cfg, board_data_cfg, bd);
	return skip(t + 1);
}



static int set_cfg(char *js, jsmntok_t * t, struct mbag_itemdef *defs, mbag_t mbag)
{

	*(js + t->end) = 0;
	const char *key = js + t->start;
	*(js + (t + 1)->end) = 0;
	//const char * val = js+(t+1)->start;



	struct mbag_itemdef *idef = get_cfg(defs, key);

//      printf("Key: %s\n",key);
//      printf("Val: %s\n",val);

	if (!idef)
		return skip(t + 1);

	if (!idef->setfun) {
		printf("Error no setfun: %s\n", key);

	} else {
		idef->setfun(idef, js, t + 1, mbag);
	}

	return skip(t + 1);

}




int setup_conf(struct conn *conn)
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
	}

	scn_obj(jstr, t, set_cfg, general_cfg, conn->config);



//void dbg_istore_dmp(mbag_t s);

//dbg_istore_dmp(conn->local);




/*

	for (i = 0; cfg[i].item_id != CW_ITEM_NONE; i++) {
		printf("ItemName: %s\n", cfg[i].cfgname);	//.cfgname)

	}
*/


	return 0;
}
