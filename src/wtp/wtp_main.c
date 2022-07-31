
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
\

#include "cw/val.h"
#include "cw/dbg.h"
#include "cw/log.h"
#include "cw/msgset.h"
#include "cw/cw.h"

#include "cw/dtls.h"
#include "wtp.h"
#include "cw/mavltypes.h"

#define MAX_MODS 32
struct bootcfg{
	const char * modnames[MAX_MODS];
	int nmods;
	const char * modpath;
	const char * cfgfilename;
};


bstr_t get_base_rmac()
{

	static  uint8_t rm[8]={0x00,0x3e,0x99,0x04,0xfa,0xd0};
	return bstr_create(rm, 6);
}



static int parse_args (int argc, char *argv[], struct bootcfg * bootcfg)
{
	int c;
	opterr = 1;
	
	bootcfg->cfgfilename = "config.ktv";
	
	while ( (c = getopt (argc, argv, "p:d:vc:m:h")) != -1) {
		
		switch (c) {
			case 'v':
				printf("WFAT version 0.0.1\n");
				exit(EXIT_SUCCESS);
				break;
			case 'd':{
				if (!cw_dbg_set_level_from_str(optarg)){
					fprintf(stderr,"Invalid debug option: %s\n",optarg);
					exit(EXIT_FAILURE);
				}
				break;
			}
			case 'm':
				if (bootcfg->nmods>MAX_MODS){
					fprintf(stderr,"Too many modules\n");
					exit(EXIT_FAILURE);
				}
				bootcfg->modnames[bootcfg->nmods++] = optarg;
				break;
			case 'p':
				cw_mod_set_path(optarg);
				break;
			case 'c':
				bootcfg->cfgfilename = optarg;
				break;
			case '?':
				exit(EXIT_FAILURE);
			default:
			case 'h':
				printf("%s: -vcmh\n",argv[0]);
				exit(EXIT_SUCCESS);
				break;
		}
	}
	return 0;
}

#include "cw/rand.h"

int main (int argc, char **argv)
{
	struct bootcfg bootcfg;
	struct cw_Mod * mod;
	struct cw_MsgSet * msgset;
	struct conn * conn;
	FILE * file;
	mavl_t global_cfg, types_tree;
	const cw_Type_t ** ti;
	int i;
	struct cw_DiscoveryResult dis;
	
	
	
	
	
	
	bootcfg.nmods=0;
	
	/* read command line args, results are in bootcfg */
	parse_args(argc,argv, &bootcfg);

	if (bootcfg.nmods==0){
		bootcfg.modnames[0]="capwap";
		bootcfg.modnames[1]="capwap80211";
		bootcfg.nmods=2;
	}
	

	/* create an empty message set */
	msgset = cw_msgset_create();
	if (msgset==NULL){
		cw_log(LOG_ERR, "Error creating msgset: %s", strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* create an empty global config */
	global_cfg=cw_ktv_create();
	if (msgset==NULL){
		cw_log(LOG_ERR, "Error creating global_cfg: %s", strerror(errno));
		exit(EXIT_FAILURE);
	}
	

	/* create a types tree with default types */
	types_tree = cw_ktv_create_types_tree();
	for (ti=CW_KTV_STD_TYPES;*ti;ti++){
		mavl_insert_ptr(types_tree,*ti);
	}

	/* read the initial config file */
	file = fopen(bootcfg.cfgfilename,"r");

	if (file == NULL){
		cw_log(LOG_ERR,"Can't open file '%s':%s",bootcfg.cfgfilename, strerror(errno));
		exit(EXIT_FAILURE);
	}

	cw_ktv_read_file(file,global_cfg,types_tree);

	
	cw_dbg_ktv_dump(global_cfg,DBG_CFG_DMP,"----- global cfg start -----","","----- global cfg end -----");
	
	
	/*clean_cfg(global_cfg);*/
	

	/* create a connection object */
	conn = conn_create_noq(-1, NULL);
	if (conn==NULL){
		cw_log(LOG_ERR, "Connot create conn: %s", strerror(errno));
		exit(EXIT_FAILURE);
	}


/*	conn->mod=mod;*/
	conn->detected = 1;
	conn->dtls_verify_peer=0;
	conn->dtls_mtu = 1200;
	conn->msgset=msgset;
	conn->local_cfg = global_cfg;
	conn->remote_cfg = NULL;
	conn->role = CW_ROLE_WTP;
	conn->wbid=1;

	for (i=0;i<bootcfg.nmods; i++){	
		mod = cw_mod_load(bootcfg.modnames[i], global_cfg, CW_ROLE_WTP);
		if (mod == NULL){
			exit (EXIT_FAILURE);
		}
		/* Build a message set from our loaded modules */
		mod->register_messages(msgset, CW_MOD_MODE_CAPWAP);
		mod->register_messages(msgset, CW_MOD_MODE_BINDINGS);
		if (mod->setup_cfg)
			mod->setup_cfg(conn);
	}
/*
{
int idx;
cw_dbg_ktv_dump(conn->local_cfg,DBG_INFO,"head","BREP: ","bot");
idx = cw_ktv_idx_get(conn->local_cfg,"tube",0,NULL);
printf("IDX: %d\n",idx);

exit(0);
}
 */
	dtls_init();

	conn->base_rmac = get_base_rmac();


	cw_discovery_init_results(&dis);
/*cw_run_discovery(conn, "255.255.255.255","192.168.56.1", &dis);*/
/*	cw_run_discovery(conn, "255.255.255.255",NULL, &dis);*/
	cw_run_discovery(conn, "192.168.0.162","192.168.0.14", &dis);
	cw_dbg_ktv_dump(dis.prio_ip, DBG_INFO, "=== IP list ===", "IP", "=== END IP List ===");


	{
		mavliter_t i;
		mavliter_init(&i, dis.prio_ip);

		mavliter_foreach(&i) {
			char ipstr[100];
			char *rk;
			cw_Val_t *val;
			mavl_t cfg;
			
			val = mavliter_get(&i);
			rk = val->key;
			val = val->val.ptr;
			val->type->to_str(val, ipstr, 100);
			printf("PTRVAL(%s): %s - %s\n", rk, val->key, ipstr);
			
			
			cfg = cw_ktv_get_sysptr(dis.prio_ac,rk,NULL);
			
			cw_dbg_ktv_dump(cfg,DBG_INFO,"----- dump of remote","dmp","endof of remote -------");
			
		}
	}

	mavl_del_all(conn->remote_cfg);
printf("JOIN\n");	
	join(conn,&dis);
	clean_cfg(conn->remote_cfg);
	mavl_merge(conn->local_cfg,conn->remote_cfg);
	
	mavl_del_all(conn->remote_cfg);
printf("JOIN CONF\n");	
	configure(conn);
	clean_cfg(conn->remote_cfg);
	mavl_merge(conn->local_cfg,conn->remote_cfg);
	
	
	changestate(conn);
	
	
	run(conn);
	
	cw_discovery_free_results(&dis);

	return (EXIT_SUCCESS);

}
