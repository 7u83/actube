#include <stdint.h>

#include "ktv.h"


static int str_getc(struct cw_KTV_Reader * r)
{
	if (r->next==r->maxlen)
		return EOF;
		
	return *((uint8_t*)(r->data)+r->next++);
}

static void str_ungetc(struct cw_KTV_Reader * r, int c)
{
	if (r->next>0)
		r->next--;
}

void cw_ktv_init_str_reader(struct cw_KTV_Reader *r, const char * str, int len)
{
	memset(r,0,sizeof(struct cw_KTV_Reader));
	r->data = str;
	r->getchar=str_getc;
	r->ungetchar=str_ungetc;
	r->maxlen=len;
}

#include <ctype.h>

#include "ktv.h"
/*
struct parser {
	int line;
	int pos;
	int prevpos;
	char error[256];
	int quote;
	FILE *f;
	int (*getc)(struct parser *);
	void (*ungetc)(struct parser *)
};
*/



static int get_char(struct cw_KTV_Reader *r)
{
	int c;
	c = r->getchar (r);
	r->pos++;
	if (c=='\n'){
		r->prevpos=r->pos;
		r->line ++;
		r->pos=0;
	}
	return c;
}


static void unget_char(struct cw_KTV_Reader * r,int c){
	r->ungetchar(r,c);
	if (c=='\n'){
		r->line--;
		r->pos=r->prevpos;
	}
	else
		r->pos--;
}



static int get_char_q(struct cw_KTV_Reader *p)
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












static int skip_chars (struct cw_KTV_Reader *r, const char * chars)
{
	int c;
	
	while ( (c = get_char (r)) != EOF) {
		if (strchr (chars, c))
			continue;
		return c;
	}
	return c;
}

static int skip_to_chars (struct cw_KTV_Reader *r, const char *chars)
{
	int c;
	
	while ( (c = get_char (r)) != EOF) {
		if (strchr (chars, c))
			return c;
	}
	return c;
}



static int read_key (struct cw_KTV_Reader *r, char *key, int max_len)
{
	int c,n;

	do {
		c = skip_chars (r, " \t\n\a\v");
		if (c == '#') {
			c = skip_to_chars (r, "\n\a");
		} else {
			break;
		}
	} while (c != EOF);
	
	unget_char(r,c);
	c=get_char_q(r);

	n=0;
	while(c!=EOF && n<max_len){
		if (!r->quote && !isalnum(c) && !strchr("._/-()@#|{}[]\\",c)){
			unget_char(r,c);
			break;
		}

		key[n]=c;
		c=get_char_q(r);
		n++;

	}
	key[n]=0;
	return n;
}


static int skip_to_colon(struct cw_KTV_Reader * r)
{
	int c;
	c = skip_chars (r, " \t");
	if (c!=':' && c!='='){
		if (c=='\n'){
			unget_char(r,c);
			sprintf(r->error,"Unexpected EOL, colon or equal sign expected.");
			return -1;
		}
		sprintf(r->error,"Colon or equal sign expected.");
		return -1;
	}
	return c;
}


static int read_type(struct cw_KTV_Reader * r, char *type, int max_len)
{
	int c,n;
	
	c = skip_to_colon(r);
	if (c==-1)
		return -1;
	if (c=='='){
		unget_char(r,c);
		return sprintf(type,"%s","Str");
		
	}

	c = skip_chars (r, " \t");
	
	if (c==':'){
		unget_char(r,c);
		sprintf(type,"%s","Str");
		return 0;
	}
		
	if (!isalpha(c)){
		if (c=='\n'){
			unget_char(r,c);
			/*sprintf(p->error,"Error at line %d, pos %d: Unexpected EOL.", p->line, p->pos);*/
			return -1;
		}
		
		/*sprintf(p->error,"Error at line %d, pos %d: Letter expected.", p->line, p->pos);*/
		return -1;
	}

	n=0;
	while(c!=EOF && n<max_len){
		if (!isalnum(c) && !strchr("_/-.()@#|{}[]",c)/*strchr(": \t\n\a",c)*/){
			unget_char(r,c);
			break;
		}

		type[n]=c;
		c=get_char(r);
		n++;
	}
	type[n]=0;
	return n;
}


static int read_val(struct cw_KTV_Reader *r, char *val, int max_len){
	int c,n,quote;
	c = skip_to_colon(r);
	if (c==-1)
		return -1;
	c = skip_chars (r, " \t");
	if (c=='"'){
		quote=1;
		c=get_char(r);
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
				c = get_char(r);
				switch(c){
					case 'n':
						c='\n';
						break;
					case '\\':
						break;
					case '"':
						break;
					default:
						unget_char(r,c);
						c='\\';
				}
			}
		}
		val[n++]=c;
		c=get_char(r);
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


/*
int cw_ktv_parse_line (FILE *f, char * key, char * type, char *val)
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
 */


int cw_ktv_parse_line(struct cw_KTV_Reader * r)
{
	
}

int cw_ktv_parse_string(struct cw_KTV_Reader *r, char *key, char *type, char *val)
{

	int n;

	

	n = read_key (r,key,CW_KTV_MAX_KEY_LEN);
	n = read_type(r,type,200);
	if (n==1)
		return -1;
	n = read_val(r,val,200);
	return n;
	
}