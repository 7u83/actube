#include "mavl.h"

/**
 * Init an AVL Tree Iterator.
 *
 * After initialization #mavliter_next would return the first element.
 * The behavior of #mavliter_get would still be undefined.
 * @param i AVL Iterator to initialize
 * @param t correspondending AVL Tree
 *
 * @See mavliter_t, 
 */
void mavliter_init(mavliter_t *i, mavl_t t){
	i->root = t->root;
	i->stack_ptr=0;
	i->cmp=t->cmp;
}


