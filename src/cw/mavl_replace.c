#include "mavl.h"

void *mavl_replace(struct mavl *t,void *data){
	struct mavlnode * node = mavl_get_node(t,data);
	if (node){
		t->del(node->data);
		return node->data=data;	
	}
	return mavl_add(t,data);
}


