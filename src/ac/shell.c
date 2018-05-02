
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "cw/sock.h"
#include "cw/log.h"
#include "cw/dbg.h"


void execute_cmd(const char *str)
{
	char cmd[1024];
	char args[1024];
	
	sscanf(str,"%s%s",cmd,args);
	printf("CMD: %s, ARGS: %s\n",cmd,args);
	


}


void shell_loop(FILE *file)
{
	int c;
/*	setvbuf(file,NULL,_IONBF,0);
	fflush(file);
*/

	char str[2048];


	do {
		fprintf(file,"actube[%d]:>",fileno(file));
		fflush(file);
		
		fgets(str,sizeof(str),file);
		execute_cmd(str);

	}while (c!=EOF);
	
}


void * run_shell(void * arg)
{
	struct sockaddr_storage server, client;
	socklen_t client_size;
	char sockstr[SOCK_ADDR_BUFSIZE];
	
	int rc;
	const char * addr = "127.0.0.1:5000";
	int sockfd, clientsock;
	int yes;
	
	rc = sock_strtoaddr(addr,(struct sockaddr*)&server);
	if (! rc ){
		cw_log(LOG_ERR,"Can't parse address '%s', %s",addr,strerror(errno));
	}

	sockfd = socket(((struct sockaddr*)&server)->sa_family,SOCK_STREAM,0);
	
	yes = 1;
	/* reuse address */
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	

	/* bind address */
	rc =  bind(sockfd,(struct sockaddr*)&server,sizeof(server));
	if (rc ){
		cw_log(LOG_ERR,"Can't bind socket address '%s', %s",addr,strerror(errno));
	}
	
	rc = listen(sockfd,5);
	if (rc ){
		cw_log(LOG_ERR,"Can't listen on address '%s', %s",addr,strerror(errno));
	}
	

	client_size = sizeof(client);
	clientsock = accept(sockfd,(struct sockaddr*)&client,&client_size);
	
	if (clientsock>0){
		sock_addr2str_p(&client,sockstr);
		cw_dbg(DBG_INFO, "Acceptiong session from %s",sockstr);
		shell_loop(fdopen(clientsock,"a+"));
		close(clientsock);
	}
	
	
	
	printf("Accepting %i, %s",rc,strerror(errno));
	
	
	

	return NULL;
}


void start_shell()
{
	pthread_t thread;
	pthread_create(&thread, NULL, run_shell,
                       NULL);
}