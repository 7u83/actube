
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
\

#include "cw/ktv.h"
#include "cw/dbg.h"
#include "cw/log.h"
#include "cw/msgset.h"


#include "wtp.h"

struct bootcfg{
	const char * modname;
	const char * modpath;
	const char * cfgfilename;
};


static int parse_args (int argc, char *argv[], struct bootcfg * bootcfg)
{
	int c;
	opterr = 1;
	
	bootcfg->modname = "capwap";
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
				bootcfg->modname = optarg;
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



int main (int argc, char **argv)
{
	struct bootcfg bootcfg;
	struct cw_Mod * mod;
	struct cw_MsgSet * msgset;
	struct conn * conn;
	FILE * file;
	mavl_t global_cfg, types_tree;
	const cw_Type_t ** ti;
	
	/* read command line args, results are in bootcfg */
	parse_args(argc,argv, &bootcfg);
	

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
		mavl_add_ptr(types_tree,*ti);
	}

	/* read the initial config file */
	file = fopen(bootcfg.cfgfilename,"r");

	if (file == NULL){
		cw_log(LOG_ERR,"Can't open file '%s':%s",bootcfg.cfgfilename, strerror(errno));
		exit(EXIT_FAILURE);
	}

	cw_ktv_read_file(file,global_cfg,types_tree);

	
	cw_dbg_ktv_dump(global_cfg,DBG_CFG_DMP,"----- global cfg start -----","","----- global cfg end -----");

	
	mod = cw_mod_load(bootcfg.modname, global_cfg, CW_ROLE_WTP);
	if (mod == NULL){
		exit (EXIT_FAILURE);
	}
		
	/* Build a message set from our loaded modules */
	mod->register_messages(msgset, CW_MOD_MODE_CAPWAP);
	mod->register_messages(msgset, CW_MOD_MODE_BINDINGS);

	/* create a connection object */
	conn = conn_create_noq(-1, NULL);
	if (conn==NULL){
		cw_log(LOG_ERR, "Connot create conn: %s", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	
	conn->detected = 1;
	conn->dtls_verify_peer=0;
	conn->dtls_mtu = 12000;
	conn->msgset=msgset;
	conn->local_cfg = global_cfg;
	conn->remote_cfg = cw_ktv_create();
	conn->receiver = CW_RECEIVER_WTP;

	cw_run_discovery(conn, "255.255.255.255",NULL);

	return (EXIT_SUCCESS);

}
