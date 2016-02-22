
#include "capwap.h"
#include "dbg.h"



struct args {
	cw_action_in_t *a;
	cw_action_in_t **out;
	struct avltree *mand;
	int n;
};
/*
static int check_mand_cb(void *priv, void *val)
{

	
	cw_action_in_t *a = (cw_action_in_t*)val;
	struct args *args = (struct args*) priv;

printf("Na?\n");
return 1;

	if ( (args->a->msg_id != a->msg_id) || (args->a->capwap_state != a->capwap_state))
	{
		printf ("Was soll das denn?\n");
		printf("End of fun\n");

		return 0;
	}

printf("Nu?\n");
return 1;

	printf("Found wat %d %d %c\n",a->msg_id,a->elem_id, a->mand ? '*':'-');
	if (a->mand) {
		int i = a->item_id;
		void * rc = avltree_del(args->mand,&i);
		if (!rc) {
			if (args->out)
				args->out[args->n]=a;
			args->n++;
		}
		
		
	}

	return 1;
}

*/

int cw_check_missing_mand(cw_action_in_t ** out, struct conn * conn, cw_action_in_t *a)
{


	cw_action_in_t as;

	as.capwap_state = a->capwap_state;
	as.msg_id = a->msg_id;
	as.vendor_id = 0;
	as.elem_id = 0;
	as.proto=0;


	DEFINE_AVLITER(it,conn->actions->in);
	int n=0;
	avliter_foreach_from(&it,&as) {
//	avliter_foreach(&it) {
		cw_action_in_t * ai = avliter_get(&it);

		if (ai->msg_id != as.msg_id && ai->capwap_state != as.capwap_state) 
			break;
		if (!ai->mand)
			continue;

		int  i = (intptr_t)ai->item_id;
		void * rc = mavl_get(conn->mand,&i);
		if (!rc) {
			out[n++]=ai;
		}
	
	}



/*

	struct args args;
	args.a = &as;
	args.mand=conn->mand;
	args.out=out;
	args.n=0;

	avltree_foreach_from_asc(conn->actions->in,&as,check_mand_cb,&args);
*/
	return n;	
}


