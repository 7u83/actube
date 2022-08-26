/*	$NetBSD: tc1.c,v 1.7 2016/02/17 19:47:49 christos Exp $	*/

/*-
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Christos Zoulas of Cornell University.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * test.c: A little test program
 */
#include <sys/wait.h>
#include <ctype.h>
#include <dirent.h>
#include <locale.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>


#include "histedit.h"

#include "cw/cfg.h"
#include "cw/sock.h"


static int continuation = 0;
volatile sig_atomic_t gotsig = 0;

static	unsigned char	complete(EditLine *, int);
static	char   *prompt(EditLine *);
static	void	sig(int);


FILE * act_f;

static int 
get_result(FILE *f, char *str,int len)
{
	int n = len;
	char *rc;
	rc = fgets (str, len, f);
	if (rc==NULL)
		return 0;

	n = strlen(str);
	if (n==1 && str[0]=='\n')
		return 0;

	return  1;
}



static char *
prompt(EditLine *el )
{
	static char str[64];
	char str2[64];
	int rc;

	fprintf(act_f,"@prompt\n");
	rc = get_result(act_f,str,64);
	while(rc){
		rc=get_result(act_f,str2,64);
	}


	return (continuation ? "> " : str);
}

static void
sig(int i)
{
	gotsig = i;
}

static unsigned char
complete(EditLine *el, int ch __attribute__((__unused__)))
{
	DIR *dd = opendir(".");
	struct dirent *dp;
	const char* ptr;
	const LineInfo *lf = el_line(el);
	size_t len;
	int res = CC_ERROR;

	/*
	 * Find the last word
	 */
	for (ptr = lf->cursor - 1;
	    !isspace((unsigned char)*ptr) && ptr > lf->buffer; ptr--)
		continue;
	len = lf->cursor - ++ptr;

	for (dp = readdir(dd); dp != NULL; dp = readdir(dd)) {
		if (len > strlen(dp->d_name))
			continue;
		if (strncmp(dp->d_name, ptr, len) == 0) {
			if (el_insertstr(el, &dp->d_name[len]) == -1)
				res = CC_ERROR;
			else
				res = CC_REFRESH;
			break;
		}
	}

	closedir(dd);
	return res;
}

const char *cfgfilename = "config.ckv";
const char *srvaddr;
const char *prgname;

static int 
parse_args (int argc, char *argv[])
{
	int c;
	opterr = 1;
	
	
	while ( (c = getopt (argc, argv, "hvc:s:")) != -1) {
		
		switch (c) {
			case 'c':
				cfgfilename = optarg;
				break;
			case 'v':
				printf("ACT shell for ACTube version 0.0.1, %s\n", SYS_ARCH);
				exit(EXIT_SUCCESS);
				break;
			case 's':
				srvaddr = optarg;
				break;

/*			case 'd':{
                                if (!cw_dbg_set_level_from_str(optarg)){
                                        fprintf(stderr,"Invalid debug option: %s\n",optarg);
                                        exit(EXIT_FAILURE);
                                }

				break;
			} */

			case '?':
				exit(EXIT_FAILURE);
			default:
			case 'h':
				printf("Usage:\n");
				printf("%s [-v | -h | -s <dst> | -c <cfg> ]\n",argv[0]);
				printf(" -c: specify config file\n");
				printf(" -s: specify adress to connect to \n");
				printf(" -v: print version info\n");

				exit(EXIT_SUCCESS);
				break;
		}
	}
	return 0;
}

int create_tcp_fd(const char *addr)
{
	struct sockaddr server;
	
	int rc;
	int sockfd;

	memset(&server,0,sizeof(server));
	rc = sock_strtoaddr (addr, (struct sockaddr*) &server);
	
	if (! rc) {
		fprintf(stderr, "Can't parse address '%s', %s\n", addr, strerror (errno));
		return -1;
	}
	
	sockfd = socket ( ( (struct sockaddr*) &server)->sa_family, SOCK_STREAM, 0);

	rc = connect(sockfd, (struct sockaddr*)&server, sizeof(server));

	if (rc ) {
		fprintf(stderr,"%s: Error, can't connt to %s %s\n",prgname,addr,strerror(errno));
		return -1;
	}

	return sockfd;	
}

static FILE *act_connect(const char *srvaddr)
{
	int fd;
	if (srvaddr==NULL){
		fprintf(stderr,"%s: Error, no address given.\n",prgname);
		return NULL;
	}
	
	fd=create_tcp_fd(srvaddr);
	if (fd==-1)
		return NULL;
	return fdopen(fd,"a+");

//	return NULL;
}




int
main(int argc , char *argv[])
{
	prgname = argv[0];	
	parse_args(argc,argv);

	act_f = act_connect(srvaddr);
	if (act_f==NULL){
		return (EXIT_FAILURE);
	}


	EditLine *el = NULL;
	int num;
	const char *buf;
	Tokenizer *tok;
#if 0
	int lastevent = 0;
#endif
	int ncontinuation;
	History *hist;
	HistEvent ev;

	(void) setlocale(LC_CTYPE, "");
	(void) signal(SIGINT, sig);
	(void) signal(SIGQUIT, sig);
	(void) signal(SIGHUP, sig);
	(void) signal(SIGTERM, sig);

	hist = history_init();		/* Init the builtin history	*/
					/* Remember 100 events		*/
	history(hist, &ev, H_SETSIZE, 100);

	tok  = tok_init(NULL);		/* Initialize the tokenizer	*/

					/* Initialize editline		*/
	el = el_init(*argv, stdin, stdout, stderr);

	el_set(el, EL_EDITOR, "emacs");	/* Default editor is vi		*/
	el_set(el, EL_SIGNAL, 0);	/* Handle signals gracefully	*/

	el_set(el, EL_PROMPT_ESC, prompt, '\1');/* Set the prompt function */

			/* Tell editline to use this history interface	*/
	el_set(el, EL_HIST, history, hist);

					/* Add a user-defined function	*/
	el_set(el, EL_ADDFN, "ed-complete", "Complete argument", complete);

					/* Bind tab to it		*/
	el_set(el, EL_BIND, "^I", "ed-complete", NULL);

	/*
	 * Bind j, k in vi command mode to previous and next line, instead
	 * of previous and next history.
	 */
	el_set(el, EL_BIND, "-a", "k", "ed-prev-line", NULL);
	el_set(el, EL_BIND, "-a", "j", "ed-next-line", NULL);

	/*
	 * Source the user's defaults file.
	 */
	el_source(el, NULL);

	while ((buf = el_gets(el, &num)) != NULL && num != 0)  {
		int ac, cc, co;
		const char **av;
		const LineInfo *li;
		li = el_line(el);
		if (gotsig) {
			(void) fprintf(stderr, "Got signal %d.\n", (int)gotsig);
			gotsig = 0;
			el_reset(el);
		}

		if (!continuation && num == 1)
			continue;

		ac = cc = co = 0;
		ncontinuation = tok_line(tok, li, &ac, &av, &cc, &co);
		if (ncontinuation < 0) {
			(void) fprintf(stderr, "Internal error\n");
			continuation = 0;
			continue;
		}

		/* Simpler */
		history(hist, &ev, continuation ? H_APPEND : H_ENTER, buf);

		continuation = ncontinuation;
		ncontinuation = 0;
		if (continuation)
			continue;

		if (strcmp(av[0], "history") == 0) {
			int rv;

			switch (ac) {
			case 1:
				for (rv = history(hist, &ev, H_LAST); rv != -1;
				    rv = history(hist, &ev, H_PREV))
					(void) fprintf(stdout, "%4d %s",
					    ev.num, ev.str);
				break;

			case 2:
				if (strcmp(av[1], "clear") == 0)
					 history(hist, &ev, H_CLEAR);
				else
					 goto badhist;
				break;

			case 3:
				if (strcmp(av[1], "load") == 0)
					 history(hist, &ev, H_LOAD, av[2]);
				else if (strcmp(av[1], "save") == 0)
					 history(hist, &ev, H_SAVE, av[2]);
				break;

			badhist:
			default:
				(void) fprintf(stderr,
				    "Bad history arguments\n");
				break;
			}
		} else if (el_parse(el, ac, av) == -1) {
			char str[2000];
			int i;
			char * sp ="";
			str[0]=0;

			for (i=0; i<ac; i++){
				fprintf(act_f,"%s%s",sp,av[i]);
				sp=" ";
			}
			fprintf(act_f,"\n");

			fflush(act_f);
			do {
				int n;
				fgets (str, 2000, act_f);
				n = strlen(str);
//				printf("LEN: %d\n",n);
				if (n==1 && str[0]=='\n')
					break;
				fprintf(stdout,"%s",str);
			}while(!feof(act_f));

			fflush(stdout);

		}

		tok_reset(tok);
	}

	el_end(el);
	tok_end(tok);
	history_end(hist);

	return (0);
}
