#include "mavl.h"


#include "dbg.h" //Tube
#include "mbag.h"

static void mavlnode_move(mavl_t m,mavl_t t, struct mavlnode *n)
{

	
	struct mavlnode * mn = mavl_get_node(m,n->data);
	if (mn) {
DBGX("replacing !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!","");
		if (m->del) {
			m->del(mn->data);
		}
		mn->data=n->data;
	}
	else{
DBGX("ODDING replacing !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!","");
		mavl_add(m,n->data);

	}

	
	free(n);
	t->count--;
}



static void mavl_merge0(mavl_t m, mavl_t t ,struct mavlnode * n)
{
	if (!n)
		return;
	mavl_merge0(m,t,n->left);
	mavl_merge0(m,t,n->right);

mbag_item_t *i = n->data;
DBGX("MBAG I %s",i->id);

	mavlnode_move(m,t,n);
}

void mavl_merge(mavl_t m, mavl_t t)
{
	mavl_merge0(m,t,t->root);		
	t->root=0;
}


