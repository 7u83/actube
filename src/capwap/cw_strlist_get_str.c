#include "capwap.h"

const char * cw_strlist_get_str(struct cw_strlist *s,int id)
{
	while ( s->id!=0){
		if (s->id==id)
			break;
		s++;
	}
	return s->str;
}

