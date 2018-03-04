#include "mavl.h"

const char * mavl_add_strdup(mavl_t t, const char * str)
{
	mavldata_t s, *result;
	s.str = strdup(str);
	if (!s.str)
		return NULL;

	result = mavl_add(t,&s);
	if (!result){
		free(s.str);
		return NULL;
	}
		
	return result->str;
}
