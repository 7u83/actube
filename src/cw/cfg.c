#include <string.h>
#include <ctype.h>

#include <mavl.h>

#include "val.h"
#include "cfg.h"


#include "dbg.h"
#include "log.h"
#include "cw.h"

static const char *nextc(const char *s)
{
	while (isdigit(*s))
		s++;
	return s;
}

static int cmp0(const char *s1, const char *s2)
{

	const char *d1, *d2;
	int i1, i2, i;

	d1 = strchr(s1, '.');
	if (d1 == NULL)
		return strcmp(s1, s2);

	d2 = strchr(s2, '.');
	if (d2 == NULL)
		return strcmp(s1, s2);

	if ((d1 - s1) != (d2 - s2))
		return strcmp(s1, s2);

	if (strncmp(s1, s2, (d1 - s1)) != 0)
		return strcmp(s1, s2);



	if (isdigit(d1[1])) {
		i1 = atoi(d1 + 1);
	} else {
		return cmp0(d1 + 1, d2 + 1);
	}

	if (isdigit(d2[1])) {
		i2 = atoi(d2 + 1);
	} else {
		return cmp0(d1 + 1, d2 + 1);
	}

	i = i1 - i2;
	if (i == 0) {
		return cmp0(nextc(d1 + 1), nextc(d2 + 1));
	}
	return i;


}

static int cmp(const void *k1, const void *k2)
{
	struct cw_Cfg_entry *e1, *e2;
	e1 = (struct cw_Cfg_entry *) k1;
	e2 = (struct cw_Cfg_entry *) k2;


	return cmp0(e1->key, e2->key);
/*	return strcmp(e1->key,e2->key);*/
}

static void del(void *ptr)
{
	struct cw_Cfg_entry *e;
	e = (struct cw_Cfg_entry *) ptr;
	free((void *) e->key);
	free((void *) e->val);
}


cw_Cfg_t *cw_cfg_create()
{
	return mavl_create(cmp, del, sizeof(struct cw_Cfg_entry));
}

int cw_cfg_set(cw_Cfg_t * cfg, const char *key, const char *val)
{
	cw_dbg(DBG_CFG_SET, "%s: %s",key,val);

	struct cw_Cfg_entry e;
	int replaced;

	e.key = cw_strdup(key);
	if (!e.key)
		return 0;
	e.val = cw_strdup(val);
	if (!e.val) {
		free((void *) e.key);
		return 0;
	}
	void *rc = mavl_replace(cfg, &e, &replaced);
	if (!rc) {
		del(&e);
		return 0;
	}

	if (!replaced)
		return 1;
	return -1;
}

const char *cw_cfg_get(cw_Cfg_t * cfg, const char *key, const char *def)
{
	struct cw_Cfg_entry e, *r;
	e.key = key;
	r = mavl_get(cfg, &e);
	if (!r)
		return def;
	return r->val;
}

const char *cw_cfg_get_l(cw_Cfg_t ** cfg, const char * key, const char *def)
{
	int i;
	struct cw_Cfg_entry e, *r;
	for(i=0; cfg[i]!=NULL; i++){
		e.key = key;
		r = mavl_get(cfg[i], &e);
		if (r!=NULL)
			return r->val;
	}
	return def;
}


const char *cw_cfg_get2(cw_Cfg_t *cfg1, cw_Cfg_t *cfg2, const char *key, const char *def)
{
	return cw_cfg_get(cfg1, key, cw_cfg_get(cfg2,key,def));
}



bstr16_t cw_cfg_get_bstr16(cw_Cfg_t * cfg, const char * key, const char *def)
{
	const char *s;

	s = cw_cfg_get(cfg,key,def);
	if(s==NULL)
		return NULL;

	return bstr16_create_from_str(s);
}

int cw_cfg_set_bstr16(cw_Cfg_t * cfg, const char * key, bstr16_t str)
{
	CW_TYPE_BSTR16->read(cfg,key,bstr16_data(str),bstr16_len(str),NULL);
	return 0;
}



void cw_cfg_dump(cw_Cfg_t * cfg)
{
	mavliter_t it;
	struct cw_Cfg_entry *e;
	mavliter_init(&it, cfg);
	mavliter_foreach(&it) {

		e = mavliter_get(&it);
		printf("%s: '%s'\n", e->key, e->val);
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
	c = fgetc(p->f);
	p->pos++;
	if (c == '\n') {
		p->prevpos = p->pos;
		p->line++;
		p->pos = 0;
	}
	return c;
}

static void unget_char(struct parser *p, int c)
{
	ungetc(c, p->f);
	if (c == '\n') {
		p->line--;
		p->pos = p->prevpos;
	} else
		p->pos--;
}



static int get_char_q(struct parser *p)
{
	int c;

	while (1) {
		c = get_char(p);
		if (c == EOF || c == '\n')
			return c;

		if (c == '"' && !p->quote) {
			p->quote = 1;
			continue;
		}
		if (c == '"' && p->quote) {
			p->quote = 0;
			continue;
		}
		break;
	}


	if (!p->quote)
		return c;

	if (c != '\\')
		return c;

	c = get_char(p);
	switch (c) {
		case EOF:
			return c;
		case 'n':
			return '\n';

		case '\\':
			return '\\';
		case '"':
			return '"';
		default:
			unget_char(p, c);
			return '\\';
	}

	/* We will never reach here */
	/* return c; */
}

static int skip_chars(struct parser *p, const char *chars)
{
	int c;

	while ((c = get_char(p)) != EOF) {
		if (strchr(chars, c))
			continue;
		return c;
	}
	return c;
}

static int skip_to_chars(struct parser *p, const char *chars)
{
	int c;

	while ((c = get_char(p)) != EOF) {
		if (strchr(chars, c))
			return c;
	}
	return c;
}



static int read_key(struct parser *p, char *key, int max_len)
{
	int c, n;

	do {
		c = skip_chars(p, " \t\n\a\v");
		if (c == '#') {
			c = skip_to_chars(p, "\n\a");

		} else {
			break;
		}
	} while (c != EOF);

	unget_char(p, c);
	c = get_char_q(p);

	n = 0;
	while (c != EOF && n < max_len) {
		if (!p->quote && !isalnum(c)
		    && !strchr("._/-()@#|{}[]\\", c) /*strchr(": \t\n\a",c) */ ) {
			unget_char(p, c);
			break;
		}

		key[n] = c;

		c = get_char_q(p);
		n++;

	}
	key[n] = 0;
	return n;
}


static int skip_to_colon(FILE * f, struct parser *p)
{
	int c;
	c = skip_chars(p, " \t");
	if (c != ':') {
		if (c == '\n') {
			unget_char(p, c);
			sprintf(p->error,
				"Error at line %d, pos %d: Unexpected EOL, collon expected.",
				p->line, p->pos);
			return 0;
		}
		sprintf(p->error, "Error at line %d, pos %d: Collon expected.",
			p->line, p->pos);
		return 0;
	}
	return 1;
}



static int read_val(struct parser *p, char *val, int max_len)
{
	int c, n, quote;
	if (!skip_to_colon(p->f, p))
		return -1;
	c = skip_chars(p, " \t");
	if (c == '"') {
		quote = 1;
		c = get_char(p);
	} else {
		quote = 0;
	}
	n = 0;
	while (c != EOF && n < max_len) {
		if (quote && c == '"') {
			break;
		}
		if (c == '\n') {
			break;
		}
		if (quote) {
			if (c == '\\') {
				c = get_char(p);
				switch (c) {
					case 'n':
						c = '\n';
						break;
					case '\\':
						break;
					case '"':
						break;
					default:
						unget_char(p, c);
						c = '\\';
				}
			}
		}
		val[n++] = c;
		c = get_char(p);
	}


	if (!quote && n > 0) {
		while (n > 0) {
			if (isspace(val[n - 1]))
				n--;
			else
				break;
		}
	}

	val[n] = 0;

	return n;

}



int cw_cfg_read_line(FILE * f, struct parser *p, char *key, char *val)
{
	int n;


	n = read_key(p, key, CW_CFG_MAX_KEY_LEN);
	if (n == 0)
		return 1;
	if (n == -1) {
		return -1;
	}

	n = read_val(p, val, CW_CFG_MAX_KEY_LEN);
	if (n == -1) {
		return -1;
	}
	return 0;
}

int cw_cfg_read_from_file(FILE * f, cw_Cfg_t * cfg)
{
	char key[CW_CFG_MAX_KEY_LEN];
	char val[2048];
	struct parser p;

	p.line = 1;
	p.pos = 0;
	p.prevpos = 0;
	p.quote = 0;
	p.f = f;

	int rc;
	int errs = 0;


	do {

		rc = cw_cfg_read_line(f, &p, key, val);
		if (rc == -1) {
			fprintf(stderr, "Error: %s\n", p.error);
			errs++;
		}


		if (rc != 0) {
			continue;
		}

		cw_cfg_set(cfg, key, val);


	} while (rc == 0);

	return errs;
}


int cw_cfg_load(const char *filename, cw_Cfg_t * cfg)
{
	int errs;
	FILE *f = fopen(filename, "rb");
	if (!f)
		return errno;
	errs = cw_cfg_read_from_file(f, cfg);
	fclose(f);

	if (errs)
		errno = EINVAL;
	return errno;
}


void cw_cfg_iter_init(cw_Cfg_t * cfg, struct cw_Cfg_iter *cfi, const char *base)
{
	struct cw_Cfg_entry search;
	search.key = base;

	mavliter_init(&(cfi->it), cfg);
	mavliter_seek(&(cfi->it), &search, 0);
	cfi->base = base;
}


const char *cw_cfg_iter_next(struct cw_Cfg_iter *cfi, const char *key)
{
	struct cw_Cfg_entry *e;
	int bl, kl;
	const char *d;

	e = mavliter_get(&(cfi->it));
	if (e == NULL)
		return NULL;


	bl = strlen(cfi->base);
	kl = strlen(e->key);

	if (bl > kl)
		return NULL;

	if (bl == kl) {
		if (strcmp(cfi->base, e->key) != 0)
			return NULL;
		else {
			mavliter_next(&(cfi->it));
			return e->val;
		}

	}
	d = strchr(e->key, '.');
	if (d == NULL)
		return NULL;

	if (d - e->key != bl)
		return NULL;

	if (strncmp(cfi->base, e->key, bl) != 0)
		return NULL;

	mavliter_next(&(cfi->it));
	return e->val;
}


int cw_cfg_get_bool(cw_Cfg_t * cfg, const char * key, const char *def)
{
	struct cw_Val v;
	const char *s = cw_cfg_get(cfg,key,def);
	CW_TYPE_BOOL->from_str(&v,s);
	return v.val.boolean;
}


uint8_t cw_cfg_get_byte(cw_Cfg_t * cfg, char *key, uint8_t def)
{
	struct cw_Val v;
	memset(&v,0,sizeof(struct cw_Val));
	const char *s = cw_cfg_get(cfg,key,NULL);
	if (s==NULL)
		return def;
	CW_TYPE_BYTE->from_str(&v,s);
	return v.val.byte;
}



uint16_t cw_cfg_get_word(cw_Cfg_t * cfg, char *key, uint16_t def)
{
	struct cw_Val v;
	const char *s = cw_cfg_get(cfg,key,NULL);
	if (s==NULL)
		return def;
	CW_TYPE_WORD->from_str(&v,s);
	return v.val.word;
}

/*
int cw_cfg_get_word(cw_Cfg_t * cfg, char *key, const char * def)
{
	const char *s;
	s=cw_cfg_get(cfg,key,def);
	if (s==NULL)
		return 0;

}
*/

void cw_cfg_set_int(cw_Cfg_t * cfg, const char * key, int val)
{
	char a[128];
	sprintf(a,"%d",val);
	cw_cfg_set(cfg,key,a);
}



int cw_cfg_get_next_index(cw_Cfg_t * cfg, const char *key)
{
	char ikey[CW_CFG_MAX_KEY_LEN];
	struct cw_Cfg_entry search, * result;
	char *d;
	
	sprintf(ikey,"%s.%d",key,65536);
		
	search.key=ikey;
	/*//result = ktvn(ktv,&search);*/
	
	result = mavl_get_last(cfg,&search);
	if (result == NULL){
		return 0;
	}
	
	d = strchr(result->key,'.');
	if (d==NULL){
		return 0;
	}
	
	if (strncmp(result->key,ikey,d-result->key)!=0)
		return 0;
	
	return atoi(d+1)+1;
}

int cw_cfg_set_val(cw_Cfg_t * cfg, const char *key, const struct cw_Type *type, const void * valguard, const uint8_t * data, int len)
{
	cw_Val_t mdata, *mresult;
	char str[2048];
	memset(&mdata,0,sizeof(cw_Val_t));
	mdata.type=type;
	mdata.valguard=valguard;
	mresult = type->get(&mdata,data,len);
	if (!mresult){
		cw_log(LOG_ERR, "Can't create cfg element for key %s of type %s: %s",
				key,type->name, strerror(errno));
		free(mdata.key);
		return 0;
	}
	type->to_str(&mdata,str,2048);
	cw_cfg_set(cfg,key,str);

	if (type->del)	
		type->del(&mdata);
	return 1;
}


void cw_cfg_copy(cw_Cfg_t *src, cw_Cfg_t *dst)
{
	mavliter_t it;
	mavliter_init(&it, src);
	mavliter_foreach(&it) {
		int exists;
		struct cw_Cfg_entry * old_elem,*e, new_elem;

		e = mavliter_get(&it);
		new_elem.key = cw_strdup(e->key);
		new_elem.val = cw_strdup(e->val);

		old_elem = mavl_insert(dst,&new_elem,&exists);
		if (!exists){
			cw_dbg(DBG_X, "New: %s: %s",new_elem.key,new_elem.val);
			continue;
		}
		if (strcmp(new_elem.val,old_elem->val)==0){
			free((void*)new_elem.key);
			free((void*)new_elem.val);
			continue;
		}

		cw_dbg(DBG_X, "Replace: %s: %s (old: %s)",new_elem.key, new_elem.val, old_elem->val);
		if(dst->del){
			dst->del(old_elem);
		}
		memcpy(old_elem,&new_elem,dst->data_size);
	}

}


void cw_cfg_destroy(cw_Cfg_t *cfg)
{
	mavl_destroy(cfg);
}


void cw_cfg_clear(cw_Cfg_t *cfg)
{
	mavl_del_all(cfg);
}


int cw_cfg_base_exists(cw_Cfg_t * cfg, const char *key)
{
        struct cw_Cfg_entry e, *result;

        e.key=key;
        result = mavl_get_first(cfg,&e);
        if (result == NULL)
                return 0;

	if (strlen(result->key)<strlen(key))
		return 0;		
	if (result->key[strlen(key)]!='/' && result->key[strlen(key)]!='.')
		return 0;
	if (strncmp(result->key,key,strlen(key))==0)
		return 1;

	return 0;

}
