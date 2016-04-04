


#include "mbag.h"
#include "log.h"


int mbag_get_upd(mbag_t b, mbag_t b_upd, const char *id, uint8_t *dst, int *found)
{
	struct mbag_item *i = mbag_get(b_upd, id);
	if (i) {
		if (!i->type->put ){
			cw_log(LOG_ERROR,"No put method for %s",i->type->name);
			return -1;
		}
		(*found)++;
		return i->type->put(i, dst);
	}
	i = mbag_get(b,id);
	if (i) {
		if (!i->type->put){
			cw_log(LOG_ERROR,"No put method for %s",i->type->name);
			return -1;
		}

		return i->type->put(i, dst);
	}

	return -1;

}
