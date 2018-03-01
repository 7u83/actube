#include "mavl.h"

void *mavl_replace_data(struct mavl *t, void *data, int len)
{
	void *df = mavl_get(t, data);
	if (!df)
		return NULL;
	memcpy(df, data, len);
	return df;
}


