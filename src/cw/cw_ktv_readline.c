#include <ctype.h>

#include "ktv.h"

struct parser {
	int line;
	int pos;
	int prevpos;
	char error[256];
	int quote;
	FILE *f;
};

static int get_char(struct parser *p)
{
	int c;
	c = fgetc (p->f);
	p->pos++;
	if (c=='\n'){
		p->prevpos=p->pos;
		p->line ++;
		p->pos=0;
	}
	return c;
}


static void unget_char(struct parser *p,int c){
	ungetc(c,p->f);
	if (c=='\n'){
		p->line--;
		p->pos=p->prevpos;
	}
	else
		p->pos--;
}



static int get_char_q(struct parser *p)
{
	int c;

	while(1) {
		c = get_char(p);
		if (c==EOF || c=='\n')
			return c;
	
		if(c=='"' && !p->quote){
			p->quote=1;
			continue;
		}
		if(c=='"' && p->quote){
			p->quote=0;
			continue;
		}
		break;
	}
	
	
	if (!p->quote)
		return c;
			
	if (c!='\\')
		return c;
			
	c = get_char(p);
	switch(c){
		case EOF:
			return c;
		case 'n':
			return '\n';
			
		case '\\':
			return '\\';
		case '"':
			return '"';
		default:
			unget_char(p,c);
			return '\\';
	}
			
	/* We will never reach here */
	/* return c;*/
}












static int skip_chars (struct parser *p, const char * chars)
{
	int c;
	
	while ( (c = get_char (p)) != EOF) {
		if (strchr (chars, c))
			continue;
		return c;
	}
	return c;
}

static int skip_to_chars (struct parser *p, const char *chars)
{
	int c;
	
	while ( (c = get_char (p)) != EOF) {
		if (strchr (chars, c))
			return c;
	}
	return c;
}



static int read_key (struct parser *p, char *key, int max_len)
{
	int c,n;

	do {
		c = skip_chars (p, " \t\n\a\v");
		if (c == '#') {
			c = skip_to_chars (p, "\n\a");
			
		} else {
			break;
		}
	} while (c != EOF);
	
	unget_char(p,c);
	c=get_char_q(p);

	n=0;
	while(c!=EOF && n<max_len){
		if (!p->quote && !isalnum(c) && !strchr("._/-()@#|{}[]\\",c)/*strchr(": \t\n\a",c)*/){
			unget_char(p,c);
			break;
		}

		key[n]=c;
		c=get_char_q(p);
		n++;

	}
	key[n]=0;
	return n;
}


static int skip_to_colon(FILE *f,struct parser * p)
{
	int c;
	c = skip_chars (p, " \t");
	if (c!=':'){
		if (c=='\n'){
			unget_char(p,c);
			sprintf(p->error,"Error at line %d, pos %d: Unexpected EOL, collon expected.", p->line, p->pos);
			return 0;
		}
		sprintf(p->error,"Error at line %d, pos %d: Collon expected.", p->line, p->pos);
		return 0;
	}
	return 1;
}


static int read_type(struct parser *p, char *type, int max_len)
{
	int c,n;
	
	if (!skip_to_colon(p->f,p))
		return -1;

	c = skip_chars (p, " \t");
	
	if (c==':'){
		unget_char(p,c);
		sprintf(type,"%s","");
		return 0;
	}
		
	if (!isalpha(c)){
		if (c=='\n'){
			unget_char(p,c);
			sprintf(p->error,"Error at line %d, pos %d: Unexpected EOL.", p->line, p->pos);
			return -1;
		}
		
		sprintf(p->error,"Error at line %d, pos %d: Letter expected.", p->line, p->pos);
		return -1;
	}

	n=0;
	while(c!=EOF && n<max_len){
		if (!isalnum(c) && !strchr("_/-.()@#|{}[]",c)/*strchr(": \t\n\a",c)*/){
			unget_char(p,c);
			break;
		}

		type[n]=c;
		c=get_char(p);
		n++;
	}
	type[n]=0;
	return n;
}


static int read_val(struct parser *p, char *val, int max_len){
	int c,n,quote;
	if (!skip_to_colon(p->f,p))
		return -1;
	c = skip_chars (p, " \t");
	if (c=='"'){
		quote=1;
		c=get_char(p);
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
				c = get_char(p);
				switch(c){
					case 'n':
						c='\n';
						break;
					case '\\':
						break;
					case '"':
						break;
					default:
						unget_char(p,c);
						c='\\';
				}
			}
		}
		val[n++]=c;
		c=get_char(p);
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
	p.quote=0;
	p.f=f;
		
	n = read_key (&p,key,CW_KTV_MAX_KEY_LEN);
	n = read_type (&p,type,CW_KTV_MAX_KEY_LEN);
	if (n==-1){
		return -1;
	}

	n = read_val (&p,val,CW_KTV_MAX_KEY_LEN);
	if (n==-1){
		return -1;
	}
	return 0;
}
