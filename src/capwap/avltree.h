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

/* 
 * yet another avl tree implementation!
 */

#ifndef __AVLTREE_H
#define __AVLTREE_H

#include <stdio.h>

struct avlnode{
	void * data;
	struct avlnode * left;
	struct avlnode * right;
	int bal;
};


struct avltree{
	struct avlnode * root;
	int (*cmp)(const void*,const void*);
	void(*del)(void*);
	int count;
};

void avlnode_destroy(struct avltree *t,struct avlnode *n);

struct avltree * avltree_create(int (*cmp)(const void*,const void*),void(*del)(void*));
void avltree_destroy(struct avltree *t);

void avltree_del_all(struct avltree *t);
void * avltree_del(struct avltree *t, void *data);
void * avltree_add(struct avltree *t, void *data);
//void * avltree_get(struct avltree *t ,void *data);
struct avlnode * avltree_get_node(struct avltree *t ,void *data);

extern int avltree_foreach_lr(struct avlnode *n, int (*callback)(void *,void *),void *cbpriv);
extern int avltree_foreach_rl(struct avlnode *n, int (*callback)(void *,void *),void *cbpriv);
int avltree_foreach_from_lr(struct avltree *t, struct avlnode *n, void *data,int (*callback)(void *,void *),void *cbpriv);


//extern void avltree_foreach(struct avltree *t, int (*callback)(void *,void*),void *cbpriv,int dir);

static inline void * avltree_get(struct avltree *t ,void *data){
	struct avlnode * n = avltree_get_node(t,data);
	if (!n)
		return NULL;
	return n->data;


}

#define avltree_find(t,d) avltree_get(t,d)
#define avltree_insert(t,d) avltree_add(t,d)
//#define avltree_walk(t,dir) avltree_foreach(t,dir)

#define avltree_foreach_asc(t,cb,priv) avltree_foreach_lr((t)->root,cb,priv)
#define avltree_foreach_desc(t,cb,priv) avltree_foreach_rl((t)->root,cb,priv)

#define avltree_foreach_from_asc(t,d,cb,priv) avltree_foreach_from_lr(t,(t)->root,d,cb,priv);


#endif


