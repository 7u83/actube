#include <ctype.h>

#include "ktv.h"

struct parser {
	int line;
	int pos;
	int prevpos;
	char error[256];
};

static int get_char(FILE * f, struct parser *p)
{
	int c;
	c = fgetc (f);
	p->pos++;
	if (c=='\n'){
		p->prevpos=p->pos;
		p->line ++;
		p->pos=0;
	}
	return c;
}

static void unget_char(int c, FILE *f, struct parser *p){
	ungetc(c,f);
	if (c=='\n'){
		p->line--;
		p->pos=p->prevpos;
	}
	else
		p->pos--;
}

static int skip_chars (FILE *f, const char * chars, struct parser * p )
{
	int c;
	
	while ( (c = get_char (f, p)) != EOF) {
		if (strchr (chars, c))
			continue;
		return c;
	}
	return c;
}

static int skip_to_chars (FILE *f, const char *chars, struct parser * p)
{
	int c;
	
	while ( (c = get_char (f, p)) != EOF) {
		if (strchr (chars, c))
			return c;
	}
	return c;
}



static int read_key (FILE *f, char *key, int max_len, struct parser * p)
{
	int c,n;

	do {
		c = skip_chars (f, " \t\n\a\v", p);
		if (c == '#') {
			c = skip_to_chars (f, "\n\a",p);
			
		} else {
			break;
		}
	} while (c != EOF);


	n=0;
	while(c!=EOF && n<max_len){
		if (!isalnum(c) && !strchr("._/-()@#|{}[]",c)/*strchr(": \t\n\a",c)*/){
			unget_char(c,f,p);
			break;
		}

		key[n]=c;
		c=get_char(f,p);
		n++;

	}
	key[n]=0;
	return n;
}


static int skip_to_colon(FILE *f,struct parser * p)
{
	int c;
	c = skip_chars (f, " \t", p);
	if (c!=':'){
		if (c=='\n'){
			unget_char(c,f,p);
			sprintf(p->error,"Error at line %d, pos %d: Unexpected EOL, collon expected.", p->line, p->pos);
			return 0;
		}
		sprintf(p->error,"Error at line %d, pos %d: Collon expected.", p->line, p->pos);
		return 0;
	}
	return 1;
}


static int read_type(FILE *f, char *type, int max_len, struct parser *p)
{
	int c,n;
	
	if (!skip_to_colon(f,p))
		return -1;

	c = skip_chars (f, " \t", p);
	
	if (c==':'){
		unget_char(c,f,p);
		sprintf(type,"%s","");
		return 0;
	}
		
	if (!isalpha(c)){
		if (c=='\n'){
			unget_char(c,f,p);
			sprintf(p->error,"Error at line %d, pos %d: Unexpected EOL.", p->line, p->pos);
			return -1;
		}
		
		sprintf(p->error,"Error at line %d, pos %d: Letter expected.", p->line, p->pos);
		return -1;
	}

	n=0;
	while(c!=EOF && n<max_len){
		if (!isalnum(c) && !strchr("_/-.()@#|{}[]",c)/*strchr(": \t\n\a",c)*/){
			unget_char(c,f,p);
			break;
		}

		type[n]=c;
		c=get_char(f,p);
		n++;
	}
	type[n]=0;
	return n;
}

static int read_val(FILE *f, char *val, int max_len, struct parser *p){
	int c,n,quote;
	if (!skip_to_colon(f,p))
		return -1;
	c = skip_chars (f, " \t", p);
	if (c=='"'){
		quote=1;
		c=get_char(f,p);
	}
	else{
		quote=0;
	}
	n=0;
	while(c!=EOF && n<max_len){
		if (quote && c=='"'){
			break;
		}
		if (c=='\n'){
			break;
		}
		if (quote){
			if (c=='\\'){
				c = get_char(f,p);
				switch(c){
					case 'n':
						c='\n';
						break;
					case '\\':
						break;
					case '"':
						break;
					default:
						unget_char(c,f,p);
						c='\\';
				}
			}
		}
		val[n++]=c;
		c=get_char(f,p);
	}
	
	
	if(!quote && n>0){
		while(n>0){
			if (isspace(val[n-1]))
				n--;
			else
				break;
		}
	}
	
	val[n]=0;

	return n;

}



int cw_ktv_read_line (FILE *f, char * key, char * type, char *val)
{
	int n;

	struct parser p;
	p.line=1;
	p.pos=0;
	p.prevpos=0;
		
	n = read_key (f,key,CW_KTV_MAX_KEY_LEN,&p);
	n = read_type (f,type,CW_KTV_MAX_KEY_LEN,&p);
	if (n==-1){
		return -1;
	}

	n = read_val (f,val,CW_KTV_MAX_KEY_LEN,&p);
	if (n==-1){
		return -1;
	}
	return 0;
}
