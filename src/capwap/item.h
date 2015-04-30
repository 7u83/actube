#ifndef __CW_ITEM_H
#define __CW_ITEM_H

#include "mbag.h"

/* Definitions for CAPWAP Items */

struct cw_item {
	/** ID */
	const char *id;
	/** Type */
	mbagtype_t type;
	
};

struct cw_item * cw_item_get_by_name(const char *name,struct cw_item *table);
struct cw_item * cw_item_get_by_id(int id,struct cw_item *table);



#endif
