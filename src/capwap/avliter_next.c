#include "avltree.h"


void * avliter_next(avliter_t *i)
{

	while ( i->stack_ptr) {
		i->stack_ptr--;
		i->cur=i->stack[i->stack_ptr];
			
		if (!i->cur)
			continue;

		if ((i->stack_ptr)&1) {
			return i->cur->data;
		}
		break;		

	}


	if (!i->cur){
		return NULL;
	}

	while(i->cur->left) {
		/* push right branch */
		i->stack[i->stack_ptr++]=i->cur->right;
		
		/* push node */
		i->stack[i->stack_ptr++]=i->cur;


		i->cur=i->cur->left;

	}

	i->stack[i->stack_ptr++]=i->cur->right;

	return i->cur->data;
}


