
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/telnet.h>



#include "cw/sock.h"
#include "cw/log.h"
#include "cw/dbg.h"


#include "cw/connlist.h"


#include "wtplist.h"

#include "ac.h"


struct shelldata{
	FILE *out;
	char prompt[1024];
	mavl_t update_cfg;
	char *history[2000];
	char line[4096];
	int pos;
	char esc[8];
	int escpos;
};

struct sockdata{
	const char *name;
	int fd;
	cw_Cfg_t * global_cfg;
};

void select_cmd(struct shelldata *sd, const char *cmd);
void list_cmd(struct shelldata *sd, const char * cmd);
void cfg_cmd(struct shelldata *sd, const char * cmd);
void ucfg_cmd(struct shelldata *sd, const char * cmd);
void set_cmd(struct shelldata *sd, const char * cmd);
void del_cmd(struct shelldata *sd, const char * cmd);
void send_cmd(struct shelldata *sd, const char * cmd);
void wlan0_cmd(struct shelldata *sd, const char * cmd);
//void show_cfg (FILE *out, mavl_t ktv);
void show_aps (FILE *out);

struct cw_Conn * find_ap(const char *name);

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
	{"wlan0",wlan0_cmd},

	
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
	struct cw_Conn * conn;
	wtplist_lock();
	conn = find_ap(sd->prompt);
	if (conn==NULL){
		fprintf(sd->out,"WTP '%s' not found\n",sd->prompt);
	}
	else {
		stop();
//		show_cfg(sd->out,conn->remote_cfg);
	}
	wtplist_unlock();
}

void ucfg_cmd(struct shelldata *sd, const char *cmd)
{
	struct cw_Conn * conn;
	stop();
//	show_cfg(sd->out,sd->update_cfg);

}

#include "wtpman.h"

void 
send_cmd(struct shelldata * sd, const char *cmd)
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
}

void 
wlan0_cmd(struct shelldata * sd, const char *cmd)
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

}

void set_cmd(struct shelldata *sd, const char *str)
{
	struct cw_Conn * conn;
	struct cw_Val_Reader r;
	char key[CW_CFG_MAX_KEY_LEN];
	char type[CW_CFG_MAX_KEY_LEN];
	char val[2048];

	stop();

//	cw_ktv_init_str_reader(&r,str,strlen(str));
	
	cw_ktv_parse_string(&r,key,type,val);
	/*cw_ktv_parse_string(key,type,val, 2048);*/
	
	fprintf(sd->out,"%s :%s: %s\n",key,type,val);
	cw_ktv_add(sd->update_cfg,key,CW_TYPE_STR,NULL,val,strlen(val));
}

void del_cmd(struct shelldata *sd, const char *str)
{
	char key[CW_CFG_MAX_KEY_LEN];
	sscanf(str,"%s",key);
	stop();
//	cw_ktv_del_sub(sd->update_cfg,key);
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

void show_aps (FILE *out)
{
	stop();
/*	struct connlist * cl;
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

		stop();		
//		result = cw_ktv_get (conn->remote_cfg, "wtp-name", NULL);
		
		if (result == NULL) {
			strcpy (wtp_name, "");
			
		} else {
			result->type->to_str (result, wtp_name, CAPWAP_MAX_WTP_NAME_LEN);
		}
		
		
		fprintf (out, "%s\t\t%s\n", addr, wtp_name);
	}
	wtplist_unlock();
	*/
}



struct cw_Conn * find_ap(const char *name)
{
stop();
/*	
	struct connlist * cl;
	mavliter_t it;

	cl = wtplist_get_connlist();
	
	
	mavliter_init (&it, cl->by_addr);
	mavliter_foreach (&it) {
		cw_Val_t * result;
		char wtp_name[CAPWAP_MAX_WTP_NAME_LEN];
		struct cw_Conn * conn;
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
	*/
	/*wtplist_unlock();*/
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
	struct cw_Val_Reader reader;
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

static int cmpansi(char * str,char * ansistr)
{
	int sl,al;
	sl = strlen(str);
	al = strlen(ansistr);


	if(sl>al){
		return 0;
	}
	if (sl<al){
		if (strncmp(str,ansistr,sl)==0)
			return -1;
	}

	if (strcmp(str,ansistr)==0)
		return 1;

	return 0;
}


static void get_line_char_mode(FILE * file, struct shelldata *sd)
{
	int c;
	struct esc_strings * es;

	es = NULL;
	sd->line[0]=0;
	sd->pos=0;
	sd->esc[0]=0;
	sd->escpos=0;

//	fprintf (file, "\xff\xfc\22");

	/* Put telnet into char mode */
	fprintf (file,"%c%c%c",IAC,WILL,TELOPT_ECHO );
	fprintf (file,"%c%c%c",IAC,WILL,TELOPT_SGA );
//	fprintf (file,"%c%c%c",IAC,DONT,TELOPT_LINEMODE );


	fprintf (file, "\n\ractube[%s]:>", sd->prompt);
	fflush (file);

	while ( (c=fgetc(file))!= EOF){
		printf ("%02x\n",c);
		if (sd->escpos){
			int i;
			int a=0;

			sd->esc[sd->escpos++]=c;
			sd->esc[sd->escpos]=0;

			for (i=0; estr[i].str != NULL; i++){
				int rc;
				rc = cmpansi(sd->esc,estr[i].str);
				if (rc==1){
					sd->esc[0]=0;
					sd->escpos=0;
					es = &estr[i];
					break;
				}
				a |= rc;
					
			}
			if (a==0){
				sd->esc[0]=0;
				sd->escpos=0;
			}else{
				continue;
			}


		}
		if (c==0x1b){
			sd->esc[sd->escpos++]=c;
			sd->esc[sd->escpos]=0;
			printf("ESC start\n");
			continue;
		}


		if (!es){
			if (c=='\r'){
				printf("CMD: %s\n",sd->line);
				fprintf (file, "\n\ractube[%s]:>", sd->prompt);
				sd->pos=0;
				sd->line[0]=0;
				continue;
			}
			if (c=='\x7f'){
				if (sd->pos==0)
					continue;

				printf("Backspace\n");
				fprintf(file,"%c %c",8,8);
				sd->line[--sd->pos]=0;
				continue;

			}
			if (c>240 || c<30){
				continue;
			}


			sd->line[sd->pos++]=c;
			sd->line[sd->pos]=0;
			printf("putout: %c %02X\n",c,c);
			fprintf(file,"%c",c);
			continue;
		}

		printf ("ES: %s\n",es->result);

		if (strcmp(es->result,"left")==0){
			if (sd->pos>0){
				sd->pos--;
				fprintf(file,es->str);
			}
		}
		if (strcmp(es->result,"right")==0){
			if (sd->line[sd->pos]!=0){
				sd->pos++;
				fprintf(file,es->str);
			}
		}

//		fprintf(file,es->str);

		es = NULL;
//		fflush(file);


	}
}



void shell_loop (FILE *file)
{
	struct shelldata sd;
	int c;
	c=0;
	/*	setvbuf(file,NULL,_IONBF,0);
		fflush(file);
	*/
	
	char str[2048];
//	sd.update_cfg = cw_ktv_create();
	
	
	sd.out = file;
	sprintf(sd.prompt,"%s","*");
	
	do {
		int c;
		get_line_char_mode(file,&sd);


		str[0]=0;
//		c=fgetc(file);
		
		printf("%c\n",c);

		fgets (str, sizeof (str), file);

		printf("My String: %s\n",str);

		execute_cmd (&sd, str);

	} while (c != EOF);
	
}


void * run_shell (void * arg)
{
	char sockstr[SOCK_ADDR_BUFSIZE];
	struct sockdata * sockdata;
	int clientsock;
        struct sockaddr_storage client;
	socklen_t client_size;

	sockdata = (struct sockdata *)arg;

	memset(&client,0,sizeof(client));
	client_size=sizeof(client);

	cw_dbg(DBG_INFO,"Starting shell, listening on: %s (sock fd: %d)",sockdata->name, sockdata->fd);

	while(1){
		clientsock = accept (sockdata->fd, (struct sockaddr*) &client, &client_size);
		if (clientsock == -1){
			cw_log (LOG_ERR, "Accept error  '%s', %s", "addr", strerror (errno));
			return NULL;

		}
		
		if (clientsock > 0) {
			sock_addr2str_p (&client, sockstr);
			cw_dbg (DBG_INFO, "Acceptiong session from %s", sockstr);
			cw_dbg (DBG_INFO, "Start shell");
			shell_loop (fdopen (clientsock, "a+"));
			cw_dbg (DBG_INFO, "Stop shell");
			close (clientsock);
		}
		
		
		
		//cw_dbg (DBG_INFO,"Accepting shell session %i, %s", rc, strerror (errno));
	}	
	
	return NULL;
}

int create_tcp_fd(const char *name)
{
	struct sockaddr_storage server, client;
	socklen_t client_size;
	char sockstr[SOCK_ADDR_BUFSIZE];
	
	int rc;
	const char * addr = name;
	int sockfd, clientsock;
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
	rc =  bind (sockfd, (struct sockaddr*) &server, sizeof (server));
	
	if (rc) {
		cw_log (LOG_ERR, "Can't bind socket address '%s', %s", addr, strerror (errno));
		return -1;
	}
	
	return sockfd;	
}
static int create_unix_fd(const char *name)
{
        struct sockaddr_storage client;
	socklen_t client_size;
	struct sockaddr_un addr;
	int rc,fd;

	unlink(name);
	fd = socket(PF_UNIX, SOCK_STREAM, 0);

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, name, sizeof(addr.sun_path)-1);
	rc = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
	if (rc) {
		cw_log (LOG_ERR, "Can't bind socket 'unix:%s', %s", name, strerror (errno));
		return -1;
	}
	int clientsock = accept (fd, (struct sockaddr*) &client, &client_size);

	return fd;
}


int start_shell(cw_Cfg_t *global_cfg)
{
	struct sockdata * sockdata;
	const char *sockname;
	int rc, type;
	int fd;

	rc = cw_cfg_get_bool(global_cfg,"actube/shell/enable",1);
	if (!rc)
		return 1;

	sockdata = malloc(sizeof(struct sockdata));
	if (sockdata==NULL)
		return 0;

	memset(sockdata,0,sizeof(struct sockdata));

	sockdata->global_cfg = global_cfg;
	sockdata->fd=-1;

	sockname = cw_cfg_get(global_cfg,"actube/shell/listen",NULL);
	
	if (sockname==NULL) {
		cw_log (LOG_ERR, "Can't get shell listen address from global_cfg 'actube/shell/listen");
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
	pthread_create (&thread, NULL, run_shell,
	                sockdata);
	return 1;
errX:
	if (sockdata->fd!=-1)
		close(sockdata->fd);
	if (sockdata->name)
		free(sockdata->name);

	free(sockdata);
	return 0;
}
