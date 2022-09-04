
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
	


bstr_t get_base_rmac()
{

	static  uint8_t rm[8]={0x00,0x3e,0x99,0x04,0xfa,0xd0};
	return bstr_create(rm, 6);
}



static int parse_args (int argc, char *argv[], struct bootcfg * bootcfg)
{
	int c;
	opterr = 1;
	
	bootcfg->config_file = "config.ckv";
	bootcfg->startup_file = "startup.ckv";
	bootcfg->local_startup_file ="startup-local.ckv";
	
	while ( (c = getopt (argc, argv, "l:s:p:d:vc:m:h")) != -1) {
		
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
				bootcfg->config_file = optarg;
				break;
			case 's':
				bootcfg->startup_file = optarg;
				break;
			case 'l':
				bootcfg->local_startup_file = optarg;
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



struct bootcfg bootcfg;


int main (int argc, char **argv)
{
	struct cw_Mod * mod;
	struct cw_MsgSet * msgset=NULL;
	struct cw_Conn * conn=NULL;
	//FILE * file;
	cw_Cfg_t * global_cfg =NULL;
	cw_Cfg_t * cfg=NULL;
	int i;
	int rc=EXIT_FAILURE;	
	struct cw_DiscoveryResults * results;
	const char *bind_addr, *disc_addr;


/*	

{
	struct cw_ValBit bits[] = {
		{0,"ess"},
		{1,"ibss"},
		{2,"cf-pollable"},
		{3,"cf-poll-request"},
		{4,"privacy"},
		{5,"short-preamble"},
		{2,"pbcc"},
		{3,"cf-poll-request"},
		{2,NULL}	
	};
	uint16_t x;
	cw_set_word(&x,1057);

	CW_TYPE_BITS->read(global_cfg,"hello",(uint8_t*)(&x),2,bits);
	exit(0);
}
*/

	bootcfg.nmods=0;
	
	/* read command line args, results are in bootcfg */
	parse_args(argc,argv, &bootcfg);

	if (bootcfg.nmods==0){
		bootcfg.modnames[0]="capwap";
		bootcfg.modnames[1]="capwap80211";
		bootcfg.nmods=2;
	}

	/* 
	 * set ths for production
	 * cw_dbg_set_level(DBG_X,0); 
	 * */
	

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
	global_cfg->name = "Global CFG";


	/* read the startup config file */
	rc = cw_cfg_load(bootcfg.startup_file,global_cfg);	
	if (rc){
		cw_log(LOG_ERR,"Can't open file '%s':%s",bootcfg.startup_file, strerror(errno));
		goto errX;
	}
	cw_dbg(DBG_INFO,"Startup '%s' loaded.",bootcfg.startup_file);

	if (access(bootcfg.local_startup_file,F_OK)==0){
		rc = cw_cfg_load(bootcfg.local_startup_file,global_cfg);
		if (rc){
			cw_log(LOG_ERR,"Can't open file '%s':%s",bootcfg.local_startup_file, strerror(errno));
			goto errX;
		}
		cw_dbg(DBG_INFO,"Local startup '%s' loaded.",bootcfg.local_startup_file);
	}

	/* Create a temp. cfg */
	cfg = cw_cfg_create();
	if (cfg==NULL)
		goto errX;

	/* read the current config file into temp. cfg */
	rc = cw_cfg_load(bootcfg.config_file,cfg);	
	if (rc){
		cw_cfg_destroy(cfg);
		cw_log(LOG_ERR,"Can't open file '%s':%s",bootcfg.config_file, strerror(errno));
		goto errX;
	}

	/* copy the temp. cfg into startup cfg - and show debug messages */
	cw_cfg_copy(cfg, global_cfg,DBG_CFG_UPDATES,"Startup CFG");

	/* create a connection object */
	conn = conn_create_noq(-1, NULL);
	if (conn==NULL){
		cw_log(LOG_ERR, "Connot create conn: %s", strerror(errno));
		rc = EXIT_FAILURE;
		goto errX;
	}

	cw_cfg_destroy(cfg);

/*	conn->mod=mod;*/
	conn->detected = 1;
	conn->dtls_verify_peer=0;
	conn->dtls_mtu = 1200;
	conn->msgset=msgset;
	conn->global_cfg = global_cfg;

	conn->cfg_list[0]=conn->update_cfg;
	conn->cfg_list[1]=conn->local_cfg;
	conn->cfg_list[2]=conn->global_cfg;
	conn->cfg_list[3]=NULL;
	
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


	bind_addr = cw_cfg_get(global_cfg,"wfat/bind-addr","0.0.0.0");
	disc_addr = cw_cfg_get(global_cfg,"wfat/discovery-addr","255.255.255.255");


	results = cw_run_discovery(conn,disc_addr,bind_addr);


	if (!results)
		goto errX;


	if (!join(conn,results)){
		cw_discovery_results_destroy(results); 
		goto errX;
	}

	configure(conn);

	cw_discovery_results_destroy(results); 
	
	
	
	changestate(conn);
	
	
	run(conn);
	rc = 0;
	printf("Goto errx 0\n");
	goto errX;

	
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
