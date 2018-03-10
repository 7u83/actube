#include "mavl.h"

void *mavl_replace(struct mavl *t,const void  *data, int * replaced){
	
	int exists;
	void * add = mavl_add(t,data,&exists);
	
	if (replaced != NULL){
		*replaced = exists;
	}

	if (!exists){
		return add;
	}
	
	if(t->del){
		t->del(add);
	}
	
	memcpy(add,data,t->data_size);
	return add;
	
/*
	struct mavlnode * node = mavl_get_node(t,data);
	if (node){
		if (t->del) 
			t->del(node+sizeof(struct mavlnode));
//		node->data=*data;
		memcpy(mavlnode_dataptr(node), data, t->data_size);
		
//		return &node->data;
		return mavlnode_dataptr(node);
	}
	return mavl_add(t,data);
*/
}


