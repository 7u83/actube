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
 * Yet another AVL Tree implementation
 */


/**
 * @addtogroup ALGOS
 * @{
 * 
 * @defgroup MAVL Mavl Tree
 * @{
 */



#ifndef __MAVL_H
#define __MAVL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>



/** Maximum AVL Tree depth.
    The number of nodes is calculated by 2^depth.
    So a value of 32 should be enough for around 4
    billion nodes. */
#define MAVL_MAX_DEPTH	32




/**
 * Defines the structure of an AVL Node.
 */
struct mavlnode {
	/** Pointer to data, that belongs to the node */
	
	/*	union mavldata data; */
	
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
	int ( *cmp ) ( const void *, const void * );
	/** Delete element function */
	void ( *del ) ( void * );
	/** Number of elements currently stored in the tree */
	int count;
	/** size of data appended to each mavlnode element.
	 * used to allocate space in #mavl_add. */
	size_t data_size;
};


/**
 * MAVL AVL Tree type
 */
typedef struct mavl * mavl_t;



/**
 * @param node node 
 */
#define mavlnode_dataptr(node) ((void*)(((uint8_t*)(node))+sizeof(struct mavlnode)))

struct mavl *mavl_create ( int ( *cmp ) ( const void *, const void * ),
                           void ( *del ) ( void * ), size_t data_size );

void *mavl_add ( struct mavl *t, const void *data, int *exists );

/*void *mavl_add ( struct mavl *t,  const void *data );*/
void * mavl_get ( struct mavl *t , const void *data );
void *mavl_del ( struct mavl *t, const void *data );
void *mavl_replace ( struct mavl *t, const void *data, int * result );
void mavl_destroy ( struct mavl *t );
void mavl_del_all ( struct mavl *t );
void mavl_merge ( mavl_t m, mavl_t t );
void mavlnode_destroy ( struct mavl *t, struct mavlnode *n );
struct mavlnode *mavlnode_get ( struct mavl *t, void *data );
struct mavlnode * mavl_get_node_cmp(struct mavl *t ,void *data,
	int ( *cmp ) ( const void *, const void * ));







extern int mavl_foreach_lr ( struct mavlnode *n, int ( *callback ) ( void *, void * ),
                             void *cbpriv );
extern int mavl_foreach_rl ( struct mavlnode *n, int ( *callback ) ( void *, void * ),
                             void *cbpriv );
int mavl_foreach_from_lr ( struct mavl *t, struct mavlnode *n, void *data,
                           int ( *callback ) ( void *, void * ), void *cbpriv );






void *mavl_replace_data ( struct mavl *t, void *data, int len );



/**
 * This macro is an alias for #mavl_get
 */
#define mavl_find(t,d) mavl_get(t,d)

#define mavl_find_ptr(tree,search_ptr) mavl_get_ptr(tree,search_ptr)
#define mavl_insert(t,d) mavl_add(t,d)


#define mavl_foreach_asc(t,cb,priv) mavl_foreach_lr((t)->root,cb,priv)
#define mavl_foreach_desc(t,cb,priv) mavl_foreach_rl((t)->root,cb,priv)

#define mavl_foreach_from_asc(t,d,cb,priv) mavl_foreach_from_lr(t,(t)->root,d,cb,priv);


struct mavliter {
	struct mavlnode *stack[MAVL_MAX_DEPTH * 2];
	
	struct mavlnode *cur;
	int stack_ptr;
	struct mavlnode * root;
	int ( *cmp ) ( const void *, const void * );
	
};
typedef struct mavliter mavliter_t;


void * mavliter_next ( mavliter_t *i );

void * mavliter_seek_set ( struct mavliter *i );

void mavliter_init ( mavliter_t *i, mavl_t t );


void * mavliter_get ( mavliter_t *i );

void * mavliter_get_ptr(mavliter_t *i);

void * mavliter_seek ( mavliter_t *i, void *d );

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

/*
#typedef mavl_t mavl_conststr_t;
#extern mavl_conststr_t mavl_create_conststr();
*/

/*
//int mavl_cmp_str ( const union mavldata *e1, const union mavldata *e2 );
*/


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



void mavl_freeptr(void *ptr);

/**
 * Create a mavl_t object which stores pointers. 
 * @param cmp a pointer to the compare function
 * @param del a pointer to a delete function
 * @return The mavl_t object or NULL if an error has occured.
 */
#define mavl_create_ptr(cmp,del)\
	mavl_create(cmp,del,sizeof(void*))
	
	
int mavl_cmpstr(const void *p1, const void *p2);
#define mavl_create_str() mavl_create_ptr(mavl_cmpstr,mavl_freeptr)
#define mavl_create_conststr() mavl_create_ptr(mavl_cmpstr,NULL)

void * mavl_get_ptr ( mavl_t tree, void * search );
void * mavl_add_ptr ( mavl_t tree, const void *ptr );

#define mavl_add_str(tree,str) mavl_add_ptr(tree,str)
#define mavl_get_str(tree,search) ((char *)(mavl_get_ptr(tree,search)))
#define mavliter_get_str(iter) ((char*)(mavliter_get_ptr(iter)))
#define mavliter_get_conststr(iter) ((const char*)(mavliter_get_ptr(iter)))

/**
 * @} MAVL
 * 
 * @} ALGOS
 * 
 */




#endif
