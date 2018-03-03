/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file
 * @brief MAVL, Mini AVL Tree, 
 * Yet another AVL Tree implementation!
 */


/** 
 * @defgroup MAVL  MAVL
 * @{
 */


#ifndef __MAVL_H
#define __MAVL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>


/**
 * @defgroup MAV_CONSTANTS Constants
 * @{
 */

/** Maximum AVL Tree depth. 
    The number of nodes is calculated by 2^depth.
    So a value of 32 should be enough for around 4 
    billion nodes. */
#define MAVL_MAX_DEPTH	32

/**
 * @}
 */

union mavldata {
	void *ptr;
	uint32_t dword;
	uint16_t word;
	uint8_t byte;
	const char *str;
};
typedef union mavldata mavldata_t;

/**
 * Defines the structure of an AVL Node.
 */
struct mavlnode {
	/** Pointer to data, thant belongs to the node */
	/* void *data; */
	union mavldata data;
	/** Pointer to left son*/
	struct mavlnode *left;
	/** Pointer to right son*/
	struct mavlnode *right;
	/** AVL balance */
	int bal;
};

/**
 * AVL Tree
 */ 
struct mavl {
	/** Pointer to root node */
	struct mavlnode *root;
	/** Compare function */
	int (*cmp) (const union mavldata *, const union mavldata *);
	/** Delete element function */
	void (*del) (union mavldata *);
	/** Number of elements currently stored in the tree */
	int count;
};

/** 
 * MAVL AVL Tree type
 */ 
typedef struct mavl * mavl_t;


/**
 * @defgroup MAVL_FUNCTIONS Functions
 * @{
 */
struct mavl *mavl_create(int (*cmp) (const union mavldata *, const union mavldata *),
			       void (*del) (union mavldata *));

void mavlnode_destroy(struct mavl *t, struct mavlnode *n);

void mavl_del_all(struct mavl *t);
union mavldata *mavl_del(struct mavl *t, union mavldata *data);
union mavldata *mavl_add(struct mavl *t, union mavldata *data);
union mavldata * mavl_get(struct mavl *t ,union mavldata *data);
struct mavlnode *mavl_get_node(struct mavl *t, union mavldata *data);
void * mavl_get_ptr(mavl_t tree, void * search);
void mavl_merge(mavl_t m, mavl_t t);


extern int mavl_foreach_lr(struct mavlnode *n, int (*callback) (void *, void *),
			      void *cbpriv);
extern int mavl_foreach_rl(struct mavlnode *n, int (*callback) (void *, void *),
			      void *cbpriv);
int mavl_foreach_from_lr(struct mavl *t, struct mavlnode *n, void *data,
			    int (*callback) (void *, void *), void *cbpriv);

/**
 * @}
 */



void *mavl_replace_data(struct mavl *t, void *data, int len);

union mavldata *mavl_replace(struct mavl *t,union mavldata *data);


void mavl_destroy(struct mavl *t);



#define mavl_find(t,d) mavl_get(t,d)
#define mavl_find_ptr(tree,search_ptr) mavl_get_ptr(tree,search_ptr)
#define mavl_insert(t,d) mavl_add(t,d)


#define mavl_foreach_asc(t,cb,priv) mavl_foreach_lr((t)->root,cb,priv)
#define mavl_foreach_desc(t,cb,priv) mavl_foreach_rl((t)->root,cb,priv)

#define mavl_foreach_from_asc(t,d,cb,priv) mavl_foreach_from_lr(t,(t)->root,d,cb,priv);


struct mavliter{
	struct mavlnode *stack[MAVL_MAX_DEPTH*2];

	struct mavlnode *cur;
	int stack_ptr;
	struct mavlnode * root;
	int (*cmp) (const union mavldata *, const union mavldata *);

};
typedef struct mavliter mavliter_t;


union mavldata * mavliter_next(mavliter_t *i);

union mavldata * mavliter_seek_set(struct mavliter *i);

void mavliter_init(mavliter_t *i, mavl_t t);

	
union mavldata * mavliter_get(mavliter_t *i);

extern union mavldata  * mavliter_seek(mavliter_t *i,void *d);

/** 
 * Define a AVL Iterator varialble and accociate it with 
 * an AVL Tree.
 * @param i Name of the variable to define
 * @param t #mavl_t Tree to associate
 * @see #mavliter_get #mavliter_foreach
 *
 * Example:
 * \code 
   mavl_t datatree = mavl_create();
 
   // fill up datatree with some values 
 
   MAVLITER_DEFINE (i,datatree);
   mavliter_foreach(&i) {
	// Get the current value
	void * value = mavliter_get(&i);
   
   }
   \endcode
 */ 
#define MAVLITER_DEFINE(iterator,mavl_obj)\
	mavliter_t iterator; mavliter_init(&iterator,mavl_obj)


/**
 * Iterate through all elements of an MAVL Object using a MAVL Iterator.
 * @param i pointer to MAVL Interator object
 */ 
#define mavliter_foreach(iterator)\
	for (mavliter_seek_set(iterator); NULL != mavliter_get(iterator); mavliter_next(iterator))

#define mavliter_foreach_from(i,from)\
	for (mavliter_seek(i,from); NULL != mavliter_get(i); mavliter_next(i))

#define mavliter_foreach_asc(iter,val) \
	while(NULL != (val = mavliter_next(iter)))


/**
 * Get the number of elements stored in a mavl object
 * @param m mavl object
 * @return Number of elemenst
 */
#define mavl_get_count(m) (m->count)

typedef mavl_t mavl_conststr_t;
extern mavl_conststr_t mavl_create_conststr();

int mavl_cmp_dword(const union mavldata *e1, const union mavldata *e2);
void mavl_free_bin(union mavldata *data);


/*
static inline void *mavl_replace_data(struct mavl *t, void *data, int len)
{
	void *df = mavl_get(t, data);
	if (!df)
		return NULL;
	memcpy(df, data, len);
	return df;
}

static inline void *mavl_replace(struct mavl *t,void *data){
	struct mavlnode * node = mavl_get_node(t,data);
	if (node){
		t->del(node->data);
		return node->data=data;	
	}
	return mavl_add(t,data);
}
*/


	
/** @} */

#endif


