#include "message_set.h"
#include "cw_types.h"

int cw_in_generic(struct conn * conn,  struct cw_ElemHandler * handler, 
		uint8_t * elem_data, int elem_len, struct sockaddr * from){
	mavldata_t data, *result;
	char str[30];
		
	result = handler->type->get(&data,elem_data,elem_len);
	result->kv.key = strdup(handler->key);
	
	handler->type->to_str(result,str,30);
	printf("Read %d-%s: %s %s\n", handler->id, handler->name, handler->key, str);
	mavl_add(conn->remote_cfg, result);
	return 0;
}
