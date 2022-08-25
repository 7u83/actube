
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>

#include <arpa/telnet.h>
#include <histedit.h>


#include "cw/sock.h"
#include "cw/log.h"
#include "cw/dbg.h"


#include "cw/connlist.h"


#include "wtplist.h"

#include "ac.h"


struct rpcdata{

	FILE *in;
	FILE *out;

	char prompt[1024];
	cw_Cfg_t * update_cfg;
	cw_Cfg_t * global_cfg;
	char *history[2000];
	char line[4096];
	int pos;
	char esc[8];
	int escpos;
	int quit;

	History *hist;
	HistEvent ev;
	Tokenizer *tok;
	EditLine *el;

};

struct sockdata{
	const char *name;
	int fd;
	cw_Cfg_t * global_cfg;
};

int select_cmd(struct rpcdata *sd, const char *cmd);
int list_cmd(struct rpcdata *sd, const char * cmd);
int cfg_cmd(struct rpcdata *sd, const char * cmd);
int ucfg_cmd(struct rpcdata *sd, const char * cmd);
int set_cmd(struct rpcdata *sd, const char * cmd);
int del_cmd(struct rpcdata *sd, const char * cmd);
int send_cmd(struct rpcdata *sd, const char * cmd);
int wlan0_cmd(struct rpcdata *sd, const char * cmd);
int exit_cmd(struct rpcdata *sd, const char * cmd);
int prompt_cmd(struct rpcdata *sd, const char * cmd);
int global_cfg_cmd(struct rpcdata *sd, const char * cmd);

//void show_cfg (FILE *out, mavl_t ktv);
int show_aps (FILE *out);

struct cw_Conn * find_ap(const char *name);

struct command{
	char * cmd;
	int (*fun)(struct rpcdata *sd, const char *cmd);
	
};

static struct command cmdlist[]={
	{"exit",exit_cmd},
	{"bumm",exit_cmd},
	{"cfg", cfg_cmd },
	{"del", del_cmd },
	{"ucfg", ucfg_cmd},
	{"list", list_cmd },
	{"select", select_cmd },
	{"send", send_cmd},
	{"set", set_cmd },
	{"wlan0",wlan0_cmd},
	{"global_cfg", global_cfg_cmd},

	{"@prompt",prompt_cmd},

	
	{NULL,NULL}
};


static void finish_cmd(FILE *f)
{
	fprintf(f,"\n");
	fflush(f);
}

int prompt_cmd(struct rpcdata *sd, const char *cmd)
{
	const char *acname = cw_cfg_get(sd->global_cfg,"capwap/ac-name","actube");

	fprintf(sd->out,"%s[%s]:>\n",acname,sd->prompt);
	finish_cmd(sd->out);
	return 0;
}


int global_cfg_cmd(struct rpcdata *sd, const char *cmd)
{
	cw_cfg_fdump(sd->out,sd->global_cfg);
	finish_cmd(sd->out);
	return 0;
}


int select_cmd(struct rpcdata *sd, const char *cmd)
{
	char ap [CAPWAP_MAX_WTP_NAME_LEN];
	sscanf(cmd,"%s",ap);
	strcpy(sd->prompt,ap);
	finish_cmd(sd->out);
	return 0;
}

int list_cmd(struct rpcdata *sd, const char *cmd)
{
	show_aps(sd->out);
	finish_cmd(sd->out);
	return 0;
}

int exit_cmd(struct rpcdata *sd, const char *cmd)
{
		//fprintf(sd->out,"Unknown command: '%s'\n\r\n\r",cmd);

	printf("Exitcmd %s\n",cmd);
	fprintf(sd->out,"END: %s\n\r",cmd);
	fflush(sd->out);
	return 1;
}



int cfg_cmd(struct rpcdata *sd, const char *cmd)
{
	struct cw_Conn * conn;
	wtplist_lock();
	conn = find_ap(sd->prompt);
	if (conn==NULL){
		fprintf(sd->out,"WTP '%s' not found\n",sd->prompt);
	}
	else {
		cw_cfg_fdump(sd->out,conn->remote_cfg);
	}
	finish_cmd(sd->out);
	wtplist_unlock();
	return 0;
}

int ucfg_cmd(struct rpcdata *sd, const char *cmd)
{
//	struct cw_Conn * conn;
	stop();
//	show_cfg(sd->out,sd->update_cfg);
	return 0;
}

#include "wtpman.h"

int
send_cmd(struct rpcdata * sd, const char *cmd)
{

	struct cw_Conn * conn;
	wtplist_lock();
	conn = find_ap(sd->prompt);
	if (conn==NULL){
		fprintf(sd->out,"WTP '%s' not found\n",sd->prompt);
	}
	else {
		conn->update_cfg=sd->update_cfg;
	}
	wtplist_unlock();
	return 0;
}

int
wlan0_cmd(struct rpcdata * sd, const char *cmd)
{
	stop();

	struct cw_Conn * conn;
	wtplist_lock();
	conn = find_ap(sd->prompt);
	if (conn==NULL){
		fprintf(sd->out,"WTP '%s' not found\n",sd->prompt);
	}
	else {
		FILE *f=fopen("wlan0.ktv","r");
//		cw_ktv_read_file(f,sd->update_cfg,conn->msgset->types_tree);
		//conn->update_cfg=sd->update_cfg;
		fclose(f);
	}
	wtplist_unlock();
	return 0;
}

int set_cmd(struct rpcdata *sd, const char *str)
{

	cw_Cfg_t *cfg;
	cfg = cw_cfg_create();

	cw_cfg_read_from_string(str,cfg);

	cw_cfg_fdump(sd->out,cfg);

	cw_cfg_destroy(cfg);

	finish_cmd(sd->out);

//	cw_ktv_init_str_reader(&r,str,strlen(str));
	
//	cw_ktv_parse_string(&r,key,type,val);
	/*cw_ktv_parse_string(key,type,val, 2048);*/
	
//	fprintf(sd->out,"%s %s\n",key,val);
//	cw_ktv_add(sd->update_cfg,key,CW_TYPE_STR,NULL,val,strlen(val));
	return 0;
}

int del_cmd(struct rpcdata *sd, const char *str)
{
	char key[CW_CFG_MAX_KEY_LEN];
	sscanf(str,"%s",key);
	stop();
//	cw_ktv_del_sub(sd->update_cfg,key);
//
	return 0;
}


/*
void show_cfg (FILE *out, mavl_t ktv)
{
	char value[500];
	struct cw_Val * data;
	mavliter_t it;
	const struct cw_Type * type;
	
	
	mavliter_init(&it,ktv);

	mavliter_foreach(&it){
		
		data = mavliter_get(&it);
		type = data->type;
		type->to_str(data,value,0);
		
		fprintf(out,"%s :%s: %s\n",data->key,type->get_type_name(data), value);
	}
	
	
}
*/


void print_mw(FILE *f, int w, const char * str)
{
	int n,i;
	fprintf(f, "%.*s",w,str);
	n=strlen(str);

	if ( n>w ){
		fprintf(f,"> ");
		return;
	}
	n = w-n;

	for(i=0;i<(n+2);i++){
		fprintf(f,"%c",' ');
	}
}


int  show_aps (FILE *out)
{
	struct connlist * cl;
	mavliter_t it;
	wtplist_lock();
	
	cl = wtplist_get_connlist();
	
	
	mavliter_init (&it, cl->by_addr);
	print_mw (out, 16, "AP Name");
	print_mw (out, 16, "AP Model");
	print_mw (out, 14, "Vendor");
	print_mw (out, 16, "IP ");
	fprintf(out,"\n");

	mavliter_foreach (&it) {
		char addr[SOCK_ADDR_BUFSIZE];
		const char *vendor;

		struct cw_Conn * conn;
		conn = mavliter_get_ptr (&it);

		print_mw(out,16,cw_cfg_get(conn->remote_cfg, "wtp-name", "Unknown"));
		print_mw(out,16,cw_cfg_get(conn->remote_cfg, "wtp-board-data/model-no", "Unknown"));
		vendor = cw_cfg_get(conn->remote_cfg, "wtp-board-data/vendor", "0");
		print_mw(out,14,vendor);	
		sock_addr2str_p (&conn->addr, addr);
		print_mw(out,16,addr);
		fprintf(out,"\n");

	}
	wtplist_unlock();
	return 0;
}



struct cw_Conn * find_ap(const char *name)
{
	
	struct connlist * cl;
	mavliter_t it;

	cl = wtplist_get_connlist();
	
	mavliter_init (&it, cl->by_addr);
	mavliter_foreach (&it) {
		const char *wtpname;
		struct cw_Conn * conn;
		conn = mavliter_get_ptr (&it);
		
		wtpname = cw_cfg_get (conn->remote_cfg, "wtp-name", NULL);
		
		if (wtpname == NULL)
			continue;
		
		if(strcmp(wtpname,name)==0){
			return conn;
		}

	}
	return NULL;
	
}


void con (FILE *out)
{
	stop();

/*	
	struct connlist * cl;
	mavliter_t it;
	
	
	
	wtplist_lock();
	
	cl = wtplist_get_connlist();
	
	
	mavliter_init (&it, cl->by_addr);
	fprintf (out, "IP\t\t\twtp-name\n");
	mavliter_foreach (&it) {
		cw_Val_t * result;
		char addr[SOCK_ADDR_BUFSIZE];
		char wtp_name[CAPWAP_MAX_WTP_NAME_LEN];
		struct cw_Conn * conn;
		conn = mavliter_get_ptr (&it);
		
		sock_addr2str_p (&conn->addr, addr);
		
		result = cw_ktv_get (conn->remote_cfg, "wtp-name", NULL);
		
		if (result == NULL) {
			strcpy (wtp_name, "");
			
		} else {
			result->type->to_str (result, wtp_name, CAPWAP_MAX_WTP_NAME_LEN);
		}
		
		
		fprintf (out, "Con!! %s\t\t%s\n", addr, wtp_name);
		
		{
			stop();

			mavl_t update;
//			update = cw_ktv_create();
//			cw_ktv_set_byte(update,"radio.255/admin-state",1);
//			conn->update_cfg=update;
		}


		fprintf(out,"\n");

	}
	wtplist_unlock();
	*/
}


struct command * find_cmd(const char *cmd)
{
	struct command * search,*result;
	
	result=NULL;
	search = cmdlist;
	while (search->cmd!=NULL){
		if (strncmp(cmd,search->cmd,strlen(cmd))==0){
			if (result==NULL)
				result = search;
			else 
				return NULL;
		}
		search ++;
	}
	return result;
}


int execute_cmd (struct rpcdata * sd, const char *str)
{
	char cmd[1024];
	char args[1024];
	int n;
	struct cw_Val_Reader reader;
	struct command * searchcmd;

	args[0]=0;
	
	n = sscanf (str, "%s", cmd);
	if (n<=0)
		return 0;
	
	searchcmd = find_cmd(cmd);
	if (searchcmd!=NULL){
		if (searchcmd->fun != NULL){
			return searchcmd->fun(sd, str+strlen(cmd));
		}
	}
	else{
		printf("unknow command\n");
		fprintf(sd->out,"Unknown command: '%s'\n",cmd);
		finish_cmd(sd->out);
	}

	return 0;
	
	
	char key[CW_CFG_MAX_KEY_LEN];
	char type[128];
	char val[2048];

	key[0]=0;
	type[0]=0;
	val[0]=0;

	stop();
//	cw_ktv_init_str_reader(&reader,str, strlen(str));
//	n = cw_ktv_parse_string(&reader, key,type,val);
	
	if (n==-1){
		int i;
		fprintf(sd->out,"Error on pos %d\n",reader.pos);
		fprintf(sd->out,"%s",str);
		for(i=0;i<reader.pos;i++){
			fprintf(sd->out," ");
		}
		fprintf(sd->out,"^\n");
		fprintf(sd->out,"%s\n",reader.error);
	}
	else{
		fprintf(sd->out,"%s :%s: %s\n", key,type,val);
	}
	return 0;
	
	
	n = sscanf (str, "%s%s", cmd, args);

	if (n<=0)
		return 0;
	/*printf("CMD: %s, ARGS:\n",cmd);*/
	
	if (strcmp (cmd, "s") == 0) {
		show_aps (sd->out);
		return 0;
	}
	
	if (strcmp (cmd, "con")==0){
		con(sd->out);
		return 0;
	}
	
	return 0;
}

struct esc_strings {
	char *str;
	char * result;
};

struct esc_strings estr[] = {
	{"\x1b[H", "home"},
	{"\x1b[F", "end"},
	{"\x1b[A", "up"},
	{"\x1b[B", "donw"},
	{"\x1b[D", "left"},
	{"\x1b[C", "right"},

	{NULL,NULL}

};





void rpc_loop (FILE *file, cw_Cfg_t *global_cfg)
{
	struct rpcdata sd;
	int c;
	c=0;

	char str[2048];
	
	sd.in = file;
	sd.out = file;	
	sd.global_cfg=global_cfg;


	sprintf(sd.prompt,"%s","*");
	sd.quit=0;

	do {

		str[0]=0;
		

		fgets (str, sizeof (str), file);

		if (execute_cmd (&sd, str)) {
			break;
		}

	} while (c != EOF);
	
}


void * run_rpc_server (void * arg)
{
	char sockstr[SOCK_ADDR_BUFSIZE];
	struct sockdata * sockdata;
	int clientsock;
        struct sockaddr_storage client;
	socklen_t client_size;

	sockdata = (struct sockdata *)arg;

	memset(&client,0,sizeof(client));
	client_size=sizeof(client);

	cw_dbg(DBG_INFO,"Starting RPC Service, listening on: %s (sock fd: %d)",sockdata->name, sockdata->fd);

	while(1){
		clientsock = accept (sockdata->fd, (struct sockaddr*) &client, &client_size);
		if (clientsock == -1){
			cw_log (LOG_ERR, "Accept error  '%s', %s", "addr", strerror (errno));
			return NULL;

		}
		
		if (clientsock > 0) {
			sock_addr2str_p (&client, sockstr);
			cw_dbg (DBG_INFO, "Accepting RPC session from %s", sockstr);
			rpc_loop (fdopen (clientsock, "a+"),sockdata->global_cfg);
			close (clientsock);
		}
		
		
		
		//cw_dbg (DBG_INFO,"Accepting shell session %i, %s", rc, strerror (errno));
	}	
	
	return NULL;
}

int create_tcp_fd(const char *name)
{
	struct sockaddr_storage server; //, client;
	
	int rc;
	const char * addr = name;
	int sockfd;
	int yes;
	
	rc = sock_strtoaddr (addr, (struct sockaddr*) &server);
	
	if (! rc) {
		cw_log (LOG_ERR, "Can't parse address '%s', %s", addr, strerror (errno));
		return -1;
	}
	
	sockfd = socket ( ( (struct sockaddr*) &server)->sa_family, SOCK_STREAM, 0);
	yes = 1;
	/* reuse address */
	setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (yes));
	
	
	/* bind address */
	rc =  bind (sockfd, (struct sockaddr*) &server, sizeof (struct sockaddr));
	
	if (rc) {
		cw_log (LOG_ERR, "Can't bind socket address '%s', %s", addr, strerror (errno));
		return -1;
	}
	
	return sockfd;	
}
static int create_unix_fd(const char *name)
{
        //struct sockaddr_storage client;
	//socklen_t client_size;
	struct sockaddr_un addr;
	int rc,fd;

	unlink(name);
	fd = socket(PF_UNIX, SOCK_STREAM, 0);

	sock_addrinit((struct sockaddr_storage*)&addr,AF_UNIX);
	strncpy(addr.sun_path, name, sizeof(addr.sun_path)-1);
	rc = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
	if (rc) {
		cw_log (LOG_ERR, "Can't bind socket 'unix:%s', %s", name, strerror (errno));
		return -1;
	}
	//int clientsock = accept (fd, (struct sockaddr*) &client, &client_size);

	return fd;
}

int start_rpc(cw_Cfg_t *global_cfg)
{
	struct sockdata * sockdata;
	const char *sockname;
	int rc; //, type;
	int fd;

	rc = cw_cfg_get_bool(global_cfg,"actube/rpc/enable",1);
	if (!rc)
		return 1;

	sockdata = malloc(sizeof(struct sockdata));
	if (sockdata==NULL)
		return 0;

	memset(sockdata,0,sizeof(struct sockdata));

	sockdata->global_cfg = global_cfg;
	sockdata->fd=-1;

	sockname = cw_cfg_get(global_cfg,"actube/rpc/listen",NULL);
	
	if (sockname==NULL) {
		cw_log (LOG_ERR, "Can't get RPC listen address from global_cfg 'actube/rpc/listen");
		goto errX;
	}

	if (strncmp("unix:",sockname,strlen("unix:"))==0){
		fd = create_unix_fd(strchr(sockname,':')+1);
		if (fd==-1)
			goto errX;
		sockdata->name=cw_strdup(sockname);
		sockdata->fd=fd;
	}else if (strncmp("tcp:",sockname,strlen("tcp:"))==0){
		fd = create_tcp_fd(strchr(sockname,':')+1);
		if (fd==-1)
			goto errX;
		sockdata->name=cw_strdup(sockname);
		sockdata->fd=fd;
	}else {
		fd = create_tcp_fd(sockname);
		if (fd==-1)
			goto errX;
		sockdata->name=cw_strdup(sockname);
		sockdata->fd=fd;
	}


	rc = listen (sockdata->fd, 5);
	if (rc) {
		cw_log (LOG_ERR, "Can't listen on address '%s', %s", "addr", strerror (errno));
		goto errX;
	}
	
	pthread_t thread;
	pthread_create (&thread, NULL, run_rpc_server,
	                sockdata);
	return 1;
errX:
	if (sockdata->fd!=-1)
		close(sockdata->fd);
	if (sockdata->name)
		free((void*)sockdata->name);

	free(sockdata);
	return 0;
}
