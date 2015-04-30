
#include "item.h"
#include "capwap_items.h"

/*
struct cw_item * cw_item_get_by_id(int id,struct cw_item *table)
{
	int i;
	while (table->id != CW_ITEM_NONE) {
		if (table->id == id)
			return table;
		table++;

	}
	return NULL;
}
*/

struct cw_item * cw_item_get_by_name(const char *id,struct cw_item *table)
{
	int i;
	while (table->id != CW_ITEM_NONE) {
		if (!strcmp(table->id,id))
			return table;
		table++;

	}
	return NULL;
}
