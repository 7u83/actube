#include "mavl.h"

union mavldata *mavl_replace(struct mavl *t,union mavldata *data){
	struct mavlnode * node = mavl_get_node(t,data);
	if (node){
		t->del(&node->data);
		node->data=*data;
		return &node->data;	
	}
	return mavl_add(t,data);
}


