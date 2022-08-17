
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
#include "cw/cfg.h"
	

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
	struct cw_MsgSet * msgset=NULL;
	struct cw_Conn * conn=NULL;
	FILE * file;
	cw_Cfg_t * global_cfg =NULL;
	const cw_Type_t ** ti;
	int i;
	int rc;	
	
	
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
	global_cfg=cw_cfg_create();
	if (msgset==NULL){
		cw_log(LOG_ERR, "Error creating global_cfg: %s", strerror(errno));
		goto errX;
	}

	/* read the initial config file */
	rc = cw_cfg_load(bootcfg.cfgfilename,global_cfg);	
	if (rc){
		cw_log(LOG_ERR,"Can't open file '%s':%s",bootcfg.cfgfilename, strerror(errno));
		goto errX;
	}


	/* create a connection object */
	conn = conn_create_noq(-1, NULL);
	if (conn==NULL){
		cw_log(LOG_ERR, "Connot create conn: %s", strerror(errno));
		rc = EXIT_FAILURE;
		goto errX;
	}


/*	conn->mod=mod;*/
	conn->detected = 1;
	conn->dtls_verify_peer=0;
	conn->dtls_mtu = 1200;
	conn->msgset=msgset;
	conn->global_cfg = global_cfg;
	//conn->remote_cfg = cw_cfg_create();
	
	conn->role = CW_ROLE_WTP;
	conn->wbid=1;

	for (i=0;i<bootcfg.nmods; i++){	
		mod = cw_mod_load(bootcfg.modnames[i], global_cfg, CW_ROLE_WTP);
		if (mod == NULL){
			rc = EXIT_FAILURE;
			goto errX;
		}

		
		/* Build a message set from our loaded modules */
		mod->register_messages(msgset, CW_MOD_MODE_CAPWAP);
		mod->register_messages(msgset, CW_MOD_MODE_BINDINGS);
		if (mod->setup_cfg)
			mod->setup_cfg(conn);
	}


	
	dtls_init();

	conn->base_rmac = get_base_rmac();


/*cw_run_discovery(conn, "255.255.255.255","192.168.56.1", &dis);*/
/*	cw_run_discovery(conn, "255.255.255.255",NULL, &dis);*/

	struct cw_DiscoveryResulsts * results;
//	cw_run_discovery(conn, "192.168.0.162","192.168.0.14", &dis);
	//cw_run_discovery(conn, "255.255.255.255","192.168.0.14", &dis);
//	cw_run_discovery(conn, "192.168.0.255","192.168.0.14", &dis);
	results = cw_run_discovery(conn, "255.255.255.255","192.168.0.14");

//	mavl_del_all(conn->remote_cfg);
	if (!join(conn,results)){
		cw_discovery_results_destroy(results); 
		goto errX;
	}

	configure(conn);

	cw_discovery_results_destroy(results); 
	rc = 0;
	printf("Goto errx 0\n");
	goto errX;

	
	clean_cfg(conn->remote_cfg);
	mavl_merge(conn->local_cfg,conn->remote_cfg);
	
	
	changestate(conn);
	
	
	run(conn);
	
//	cw_discovery_free_results(&dis);

	return (EXIT_SUCCESS);
errX:
	if (msgset)
		cw_msgset_destroy(msgset);

	if (global_cfg != NULL)
		cw_cfg_destroy(global_cfg);
	if (conn)
		conn_destroy(conn);


	return rc;

}
