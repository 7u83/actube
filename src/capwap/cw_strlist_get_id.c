#include <string.h>

#include "strlist.h"

int cw_strlist_get_id(struct cw_str *s,const char *str)
{
	while ( s->id!=CW_STR_STOP){
		if (strcmp(s->str,str)==0)
			break;
		s++;
	}
	return s->id;
}
