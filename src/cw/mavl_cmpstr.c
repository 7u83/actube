#include "mavl.h"
#include "mavltypes.h"

#include <string.h>

int mavl_cmpstr (const void *s1, const void *s2)
{
	return strcmp( *((const char**)s1), *((const char**)s2));
}
