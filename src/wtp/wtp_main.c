
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
\

#include "cw/ktv.h"
#include "cw/dbg.h"
#include "cw/log.h"
#include "cw/msgset.h"

struct bootcfg{
	const char * modname;
	const char * modpath;
	const char * cfgfilename;
};

static int parse_args (int argc, char *argv[], struct bootcfg * bootcfg)
{
	int c;
	opterr = 1;
	
	bootcfg->modname = "cisco";
	bootcfg->cfgfilename = "config.ktv";
	
	while ( (c = getopt (argc, argv, "p:d:vc:m:h")) != -1) {
		
		switch (c) {
			case 'v':
				printf("WFAT version 0.0.1\n");
				exit(EXIT_SUCCESS);
				break;
			case 'd':{
				int b = cw_strlist_get_id(cw_dbg_strings, optarg);
				if (b==-1){
					fprintf(stderr,"Invalid debug option: %s\n",optarg);
					exit(EXIT_FAILURE);
				}
				cw_dbg_set_level(b, 1);
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
	mavl_t types;
	mavliter_t it;
	struct bootcfg bootcfg;
	struct cw_Mod * mod;
	struct cw_MsgSet * msgset;
	
	parse_args(argc,argv, &bootcfg);
	

	msgset = cw_msgset_create();
	if (msgset==NULL){
		cw_log(LOG_ERR, "Error creating msgset: %s", strerror(errno));
		exit(EXIT_FAILURE);
	}
	mod = cw_mod_load(bootcfg.modname);
	if (mod == NULL){
		exit (EXIT_FAILURE);
	}
	
	mod->register_messages(msgset, CW_MOD_MODE_CAPWAP);
	mod->register_messages(msgset, CW_MOD_MODE_BINDINGS);
	


	types = cw_ktv_create_types_tree();
	if (types == NULL){
		perror("Error creating types tree");
		exit(EXIT_FAILURE);
	}
	
	


	
	mavl_add_ptr(types,CW_TYPE_BSTR16);
	mavl_add_ptr(types,CW_TYPE_DWORD);
	
	mavliter_init(&it,types);
	mavliter_foreach(&it){
		struct cw_Type * t = mavliter_get_ptr(&it);
		printf("The Type is %s\n",t->name);
	}
	
	return (EXIT_SUCCESS);
}
