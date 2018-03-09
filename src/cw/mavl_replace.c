#include "mavl.h"

union mavldata *mavl_replace(struct mavl *t,void  *data){
	struct mavlnode * node = mavl_get_node(t,data);
	if (node){
		if (t->del) 
			t->del(node+sizeof(struct mavlnode));
/*		node->data=*data;
*/		mavlnode_copy_data(node+sizeof(struct mavlnode), data);
		return &node->data;
	}
	return mavl_add(t,data);
}


