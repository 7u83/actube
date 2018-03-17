
#include "strlist.h"

const char * cw_strlist_get_str(struct cw_StrListElem *s,int id)
{
	while ( s->id!=CW_STR_STOP){
		if (s->id==id)
			break;
		s++;
	}
	return s->str;
}

