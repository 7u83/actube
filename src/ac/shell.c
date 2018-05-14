
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "cw/sock.h"
#include "cw/log.h"
#include "cw/dbg.h"


#include "cw/connlist.h"


#include "wtplist.h"




struct shelldata{
	FILE *out;
	char prompt[1024];
	mavl_t update_cfg;
};

void select_cmd(struct shelldata *sd, const char *cmd);
void list_cmd(struct shelldata *sd, const char * cmd);
void cfg_cmd(struct shelldata *sd, const char * cmd);
void ucfg_cmd(struct shelldata *sd, const char * cmd);
void set_cmd(struct shelldata *sd, const char * cmd);
void del_cmd(struct shelldata *sd, const char * cmd);
void send_cmd(struct shelldata *sd, const char * cmd);

struct conn * find_ap(const char *name);

struct command{
	char * cmd;
	void (*fun)();
};

static struct command cmdlist[]={
	{"cfg", cfg_cmd },
	{"del", del_cmd },
	{"ucfg", ucfg_cmd},
	{"list", list_cmd },
	{"select", select_cmd },
	{"send", send_cmd},
	{"set", set_cmd },

	
	{NULL,NULL}
};


void select_cmd(struct shelldata *sd, const char *cmd)
{
	char ap [CAPWAP_MAX_WTP_NAME_LEN];
	sscanf(cmd,"%s",ap);
	strcpy(sd->prompt,ap);
}

void list_cmd(struct shelldata *sd, const char *cmd)
{
	show_aps(sd->out);
}

void cfg_cmd(struct shelldata *sd, const char *cmd)
{
	struct conn * conn;
	wtplist_lock();
	conn = find_ap(sd->prompt);
	if (conn==NULL){
		fprintf(sd->out,"WTP '%s' not found\n",sd->prompt);
	}
	else {
		show_cfg(sd->out,conn->remote_cfg);
	}
	wtplist_unlock();
}

void ucfg_cmd(struct shelldata *sd, const char *cmd)
{
	struct conn * conn;
	show_cfg(sd->out,sd->update_cfg);

}

#include "wtpman.h"

send_cmd(struct shelldata * sd, const char *cmd)
{
	struct conn * conn;
	wtplist_lock();
	conn = find_ap(sd->prompt);
	if (conn==NULL){
		fprintf(sd->out,"WTP '%s' not found\n",sd->prompt);
	}
	else {
		conn->update_cfg=sd->update_cfg;
	}
	wtplist_unlock();
}

void set_cmd(struct shelldata *sd, const char *str)
{
	struct conn * conn;
	struct cw_KTV_Reader r;
	char key[CW_KTV_MAX_KEY_LEN];
	char type[CW_KTV_MAX_KEY_LEN];
	char val[2048];
	cw_ktv_init_str_reader(&r,str,strlen(str));
	
	cw_ktv_parse_string(&r,key,type,val);
	/*cw_ktv_parse_string(key,type,val, 2048);*/
	
	fprintf(sd->out,"%s :%s: %s\n",key,type,val);
	cw_ktv_add(sd->update_cfg,key,CW_TYPE_STR,NULL,val,strlen(val));
}

void del_cmd(struct shelldata *sd, const char *str)
{
	char key[CW_KTV_MAX_KEY_LEN];
	sscanf(str,"%s",key);
	cw_ktv_del_sub(sd->update_cfg,key);
}



void show_cfg (FILE *out, mavl_t ktv)
{
	char value[500];
	struct cw_KTV * data;
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

void show_aps (FILE *out)
{
	struct connlist * cl;
	mavliter_t it;
	wtplist_lock();
	
	cl = wtplist_get_connlist();
	
	
	mavliter_init (&it, cl->by_addr);
	fprintf (out, "IP\t\t\twtp-name\n");
	mavliter_foreach (&it) {
		cw_KTV_t * result;
		char addr[SOCK_ADDR_BUFSIZE];
		char wtp_name[CAPWAP_MAX_WTP_NAME_LEN];
		struct conn * conn;
		conn = mavliter_get_ptr (&it);
		
		sock_addr2str_p (&conn->addr, addr);
		
		result = cw_ktv_get (conn->remote_cfg, "wtp-name", NULL);
		
		if (result == NULL) {
			strcpy (wtp_name, "");
			
		} else {
			result->type->to_str (result, wtp_name, CAPWAP_MAX_WTP_NAME_LEN);
		}
		
		
		fprintf (out, "%s\t\t%s\n", addr, wtp_name);
	}
	wtplist_unlock();
}



struct conn * find_ap(const char *name)
{
	struct connlist * cl;
	mavliter_t it;

	/*wtplist_lock();*/
	cl = wtplist_get_connlist();
	
	
	mavliter_init (&it, cl->by_addr);
	mavliter_foreach (&it) {
		cw_KTV_t * result;
		char wtp_name[CAPWAP_MAX_WTP_NAME_LEN];
		struct conn * conn;
		conn = mavliter_get_ptr (&it);
		
		result = cw_ktv_get (conn->remote_cfg, "wtp-name", NULL);
		
		if (result == NULL) {
			strcpy (wtp_name, "");
			
		} else {
			result->type->to_str (result, wtp_name, CAPWAP_MAX_WTP_NAME_LEN);
		}
	
		printf("cmp '%s' : '%s'\n",wtp_name,name);
		
		if(strcmp(wtp_name,name)==0){
			return conn;
		}

	}
	return NULL;
	/*wtplist_unlock();*/
}


void con (FILE *out)
{
	struct connlist * cl;
	mavliter_t it;
	
	
	
	wtplist_lock();
	
	cl = wtplist_get_connlist();
	
	
	mavliter_init (&it, cl->by_addr);
	fprintf (out, "IP\t\t\twtp-name\n");
	mavliter_foreach (&it) {
		cw_KTV_t * result;
		char addr[SOCK_ADDR_BUFSIZE];
		char wtp_name[CAPWAP_MAX_WTP_NAME_LEN];
		struct conn * conn;
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
			mavl_t update;
			update = cw_ktv_create();
			cw_ktv_set_byte(update,"radio.255/admin-state",1);
			conn->update_cfg=update;
		}


		fprintf(out,"\n");

	}
	wtplist_unlock();
}


struct command * find_cmd(const char *cmd)
{
	struct command * search;
	
	search = cmdlist;
	while (search->cmd!=NULL){
		if (strncmp(cmd,search->cmd,strlen(cmd))==0){
			return search;
		}
		search ++;
	}
	return NULL;
}


void execute_cmd (struct shelldata * sd, const char *str)
{
	char cmd[1024];
	char args[1024];
	int n;
	struct cw_KTV_Reader reader;
	struct command * searchcmd;

	args[0]=0;
	
	n = sscanf (str, "%s", cmd);
	if (n<=0)
		return;
	
	searchcmd = find_cmd(cmd);
	
	if (searchcmd!=NULL){
		if (searchcmd->fun != NULL){
			fprintf(sd->out,"%s %s\n", searchcmd->cmd,str+strlen(cmd));
			searchcmd->fun(sd, str+strlen(cmd));
		}
	}
	else{
		fprintf(sd->out,"Unknown command: '%s'\n",cmd);
	}

	return;
	
	
	char key[CW_KTV_MAX_KEY_LEN];
	char type[128];
	char val[2048];

	key[0]=0;
	type[0]=0;
	val[0]=0;
	cw_ktv_init_str_reader(&reader,str, strlen(str));
	n = cw_ktv_parse_string(&reader, key,type,val);
	
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
	return;
	
	
	n = sscanf (str, "%s%s", cmd, args);

	if (n<=0)
		return;
	/*printf("CMD: %s, ARGS:\n",cmd);*/
	
	if (strcmp (cmd, "s") == 0) {
		show_aps (sd->out);
		return;
	}
	
	if (strcmp (cmd, "con")==0){
		con(sd->out);
		return;
	}

}


void shell_loop (FILE *file)
{
	struct shelldata sd;
	int c;
	/*	setvbuf(file,NULL,_IONBF,0);
		fflush(file);
	*/
	
	char str[2048];
	sd.update_cfg = cw_ktv_create();
	
	
	
	sd.out = file;
	sprintf(sd.prompt,"%s","*");
	
	do {
		fprintf (file, "actube[%s]:>", sd.prompt);
		fflush (file);
		str[0]=0;
		fgets (str, sizeof (str), file);
		execute_cmd (&sd, str);
		
	} while (c != EOF);
	
}


void * run_shell (void * arg)
{
	struct sockaddr_storage server, client;
	socklen_t client_size;
	char sockstr[SOCK_ADDR_BUFSIZE];
	
	int rc;
	const char * addr = "127.0.0.1:5000";
	int sockfd, clientsock;
	int yes;
while(1){
	

	rc = sock_strtoaddr (addr, (struct sockaddr*) &server);
	
	if (! rc) {
		cw_log (LOG_ERR, "Can't parse address '%s', %s", addr, strerror (errno));
	}
	
	sockfd = socket ( ( (struct sockaddr*) &server)->sa_family, SOCK_STREAM, 0);
	
	yes = 1;
	/* reuse address */
	setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (yes));
	
	
	/* bind address */
	rc =  bind (sockfd, (struct sockaddr*) &server, sizeof (server));
	
	if (rc) {
		cw_log (LOG_ERR, "Can't bind socket address '%s', %s", addr, strerror (errno));
	}
	
	rc = listen (sockfd, 5);
	
	if (rc) {
		cw_log (LOG_ERR, "Can't listen on address '%s', %s", addr, strerror (errno));
	}
	
	
	client_size = sizeof (client);
	clientsock = accept (sockfd, (struct sockaddr*) &client, &client_size);
	
	if (clientsock > 0) {
		sock_addr2str_p (&client, sockstr);
		cw_dbg (DBG_INFO, "Acceptiong session from %s", sockstr);
		printf("New shell lopp\n");
		shell_loop (fdopen (clientsock, "a+"));
		printf("end shell lopp\n");
		close (clientsock);
		printf("close clsock\n");
	}
	
	
	
	cw_dbg (DBG_INFO,"Accepting shell session %i, %s", rc, strerror (errno));
}	
	
	
	
	return NULL;
}


void start_shell()
{
	pthread_t thread;
	pthread_create (&thread, NULL, run_shell,
	                NULL);
}
