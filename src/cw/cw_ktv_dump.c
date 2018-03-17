#include "ktv.h"
#include "dbg.h"


void cw_ktv_dump(mavl_t ktv, uint32_t dbglevel, 
		const char *header, const char *prefix, const char *footer )
{
	char value[500];
	struct cw_KTV * data;
	mavliter_t it;
	const struct cw_Type * type;
	
	if (header != NULL)
		cw_dbg (dbglevel, header);
	
	mavliter_init(&it,ktv);

	mavliter_foreach(&it){
		
		data = mavliter_get(&it);
		type = data->type;
		type->to_str(data,value,0);
		
		cw_dbg(dbglevel,"%s%s (%s): %s",prefix,data->key,type->name, value);
	}
	
	if (footer != NULL)
		cw_dbg (dbglevel, footer);

}
