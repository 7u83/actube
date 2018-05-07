
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
		
		fprintf(out,"================= Local CFG: =================== \n");
		show_cfg(out,conn->local_cfg);
		fprintf(out,"================= Remote CFG: ================== \n");
		show_cfg(out,conn->remote_cfg);

		fprintf(out,"\n");

	}
	wtplist_unlock();
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


void execute_cmd (FILE * out, const char *str)
{
	char cmd[1024];
	char args[1024];
	int n;

	
	n = sscanf (str, "%s%s", cmd, args);

	if (n<=0)
		return;
	/*printf("CMD: %s, ARGS:\n",cmd);*/
	
	if (strcmp (cmd, "s") == 0) {
		show_aps (out);
		return;
	}
	
	if (strcmp (cmd, "con")==0){
		con(out);
		return;
	}

}


void shell_loop (FILE *file)
{
	int c;
	/*	setvbuf(file,NULL,_IONBF,0);
		fflush(file);
	*/
	
	char str[2048];
	
	
	do {
		fprintf (file, "actube[%d]:>", fileno (file));
		fflush (file);
		str[0]=0;
		fgets (str, sizeof (str), file);
		execute_cmd (file, str);
		
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
		shell_loop (fdopen (clientsock, "a+"));
		close (clientsock);
	}
	
	
	
	printf ("Accepting %i, %s", rc, strerror (errno));
	
	
	
	
	return NULL;
}


void start_shell()
{
	pthread_t thread;
	pthread_create (&thread, NULL, run_shell,
	                NULL);
}
