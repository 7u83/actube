#include <string.h>
#include <ctype.h>

#include <mavl.h>

#include "cw.h"
#include "cfg.h"
#include "val.h"

static int cmp(const void *k1,const void*k2){
	struct cw_Cfg_entry * e1,*e2;
	e1=(struct cw_Cfg_entry *)k1;
	e2=(struct cw_Cfg_entry *)k2;
	return strcmp(e1->key,e2->key);
}

static void del(void *ptr)
{
	struct cw_Cfg_entry * e;
	e=(struct cw_Cfg_entry *)ptr;
	free(e->key);
	free(e->val);
}


cw_Cfg_t * cw_cfg_create()
{
	return mavl_create(cmp, del, sizeof(struct cw_Cfg_entry));
}	

int cw_cfg_set(cw_Cfg_t * cfg,const char *key, const char *val)
{
	struct cw_Cfg_entry e;
	int replaced;

	e.key = cw_strdup(key);
	if (!e.key)
		return 0;
	e.val = cw_strdup(val);
	if (!e.val){
		free(e.key);
		return 0;
	}
	void * rc = mavl_replace(cfg,&e,&replaced);
	if (!rc){
		del(&e);
		return 0;
	}

	if (!replaced)
		return 1;
	return -1;
}

char * cw_cfg_get(cw_Cfg_t * cfg, char *key)
{
	struct cw_Cfg_entry e,*r;
	e.key = key;
	r = mavl_get(cfg,&e);
	if (!r)
		return NULL;
	return r->val;
}


uint16_t cw_cfg_get_word(cw_Cfg_t * cfg, char *key)
{

}


void cw_cfg_dump(cw_Cfg_t *cfg)
{
	mavliter_t it;
	struct cw_Cfg_entry *e;
	mavliter_init(&it,cfg);
	mavliter_foreach(&it){
		
		e = mavliter_get(&it);
		printf("%s: '%s'\n",e->key,e->val);
		//cw_dbg(dbglevel,"%s%s :%s: %s",prefix,data->key,type->get_type_name(data), value);
	}
}


struct parser {
	int line;
	int pos;
	int prevpos;
	char error[256];
	int quote;
	FILE *f;
	int (*getc)(struct parser *);
	void (*ungetc)(struct parser *);
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



int cw_cfg_read_line (FILE *f, struct parser *p, char * key, char *val)
{
	int n;

	
	n = read_key (p,key,CW_CFG_MAX_KEY_LEN);
	if (n==0)
		return 1;
	if (n==-1){
		return -1;
	}

	n = read_val (p,val,CW_CFG_MAX_KEY_LEN);
	if (n==-1){
		return -1;
	}
	return 0;
}

int cw_cfg_read_from_file(FILE * f, cw_Cfg_t * cfg)
{
	char key[CW_CFG_MAX_KEY_LEN];
	char val[2048];
	struct parser p;

	p.line=1;
	p.pos=0;
	p.prevpos=0;
	p.quote=0;
	p.f=f;
	
	int rc;

	
	do {

		rc = cw_cfg_read_line(f,&p,key,val);
		if (rc==-1){
			fprintf(stderr,"Error: %s",p.error);
		}


		if (rc != 0){
			continue;
		}

		cw_cfg_set(cfg,key,val);
	

	}while(rc==0);
	
	return 0;
}


int cw_cfg_load(const char *filename,cw_Cfg_t * cfg)
{
	FILE *f = fopen(filename,"rb");
	if (!f)
		return errno;
	cw_cfg_read_from_file(f,cfg);
	return 0;
}
