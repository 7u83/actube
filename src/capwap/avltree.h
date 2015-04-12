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
 * @brief Yet another avl tree implementation!
 */

#ifndef __AVLTREE_H
#define __AVLTREE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>

/** Maximum AVL Tree depth. 
    The number of nodes is calculated by 2^depth.
    So a value of 32 should be enough for around 4 
    billion nodes. */
#define AVLTREE_MAX_DEPTH	32

/**
 * Defines the structure of an AVL Node.
 */
struct avlnode {
	union {
		void *data;
		uint8_t ubyte;
		uint16_t uword;
		uint32_t udword;
		int8_t byte;
		int16_t word;
		int32_t dword;
	};
	struct avlnode *left;
	struct avlnode *right;
	int bal;
};

/**
 * AVL Tree
 */ 
struct avltree {
	struct avlnode *root;
	int (*cmp) (const void *, const void *);
	void (*del) (void *);
	int count;
};

typedef struct avltree * avltree_t;


void avlnode_destroy(struct avltree *t, struct avlnode *n);

struct avltree *avltree_create(int (*cmp) (const void *, const void *),
			       void (*del) (void *));
//void avltree_destroy(struct avltree *t);

void avltree_del_all(struct avltree *t);
void *avltree_del(struct avltree *t, void *data);
void *avltree_add(struct avltree *t, void *data);
void * avltree_get(struct avltree *t ,void *data);
struct avlnode *avltree_get_node(struct avltree *t, void *data);

extern int avltree_foreach_lr(struct avlnode *n, int (*callback) (void *, void *),
			      void *cbpriv);
extern int avltree_foreach_rl(struct avlnode *n, int (*callback) (void *, void *),
			      void *cbpriv);
int avltree_foreach_from_lr(struct avltree *t, struct avlnode *n, void *data,
			    int (*callback) (void *, void *), void *cbpriv);


//extern void avltree_foreach(struct avltree *t, int (*callback)(void *,void*),void *cbpriv,int dir);

static inline void *avltree_replace_data(struct avltree *t, void *data, int len)
{
	void *df = avltree_get(t, data);
	if (!df)
		return NULL;
	memcpy(df, data, len);
	return df;
}

static inline void *avltree_replace(struct avltree *t,void *data){
	struct avlnode * node = avltree_get_node(t,data);
	if (node){
		t->del(node->data);
		return node->data=data;	
	}
	return avltree_add(t,data);
}


static inline void avltree_destroy(struct avltree *t)
{
	avltree_del_all(t);
	free (t);
}




#define avltree_find(t,d) avltree_get(t,d)
#define avltree_insert(t,d) avltree_add(t,d)
//#define avltree_walk(t,dir) avltree_foreach(t,dir)

#define avltree_foreach_asc(t,cb,priv) avltree_foreach_lr((t)->root,cb,priv)
#define avltree_foreach_desc(t,cb,priv) avltree_foreach_rl((t)->root,cb,priv)

#define avltree_foreach_from_asc(t,d,cb,priv) avltree_foreach_from_lr(t,(t)->root,d,cb,priv);


struct avliter{
	struct avlnode *stack[AVLTREE_MAX_DEPTH*2];

	struct avlnode *cur;
	int stack_ptr;
	struct avlnode * root;
	int (*cmp) (const void *, const void *);

};
typedef struct avliter avliter_t;


void * avliter_next(avliter_t *i);

static inline void * avliter_seek_set(struct avliter *i)
{
	i->stack_ptr=0;
	i->cur=i->root;
	return avliter_next(i);
}

/**
 * Init an AVL Tree Iterator.
 *
 * After initialization #avliter_next would return the first element.
 * The behavior of #avliter_get would still be undefined.
 * @param i AVL Iterator to initialize
 * @param t correspondending AVL Tree
 *
 * @See avliter_t, 
 */
static inline void avliter_init(avliter_t *i, avltree_t t){
	i->root = t->root;
	i->stack_ptr=0;
	i->cmp=t->cmp;
}

	
/**
 * Get the element, where AVL Iterator currently is positioned.
 * @param i AVL Iterator
 * @return element or NULL if not found.
 */
static inline void * avliter_get(avliter_t *i){
	if(!i->cur)
		return NULL;
	return i->cur->data;	
}


extern void * avliter_seek(avliter_t *i,void *d);


#define DEFINE_AVLITER(i,t)\
	avliter_t i; avliter_init(&i,t)


#define avliter_foreach(i)\
	for (avliter_seek_set(i); NULL != avliter_get(i); avliter_next(i))

#define avliter_foreach_from(i,from)\
	for (avliter_seek(i,from); NULL != avliter_get(i); avliter_next(i))

#define avliter_foreach_asc(iter,val) \
	while(NULL != (val = avliter_next(iter)))

	

#endif


