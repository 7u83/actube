#ifndef __MLIST_H
#define __MLIST_H

struct mlist_elem {
	void *data;
	struct mlist_elem *next;
};

struct mlist {
	void *data;
	int (*cmp) (void *d1, void *d2);
	struct mlist_elem *list;
};

typedef struct mlist mlist_t;


extern mlist_t *mlist_create(int (*cmp) (void *v1, void *v2));
extern struct mlist_elem *mlist_append(mlist_t * l, void *data);
extern struct mlist_elem *mlist_find(mlist_t * l, struct mlist_elem *start, void *data);
extern struct mlist_elem *mlist_replace(mlist_t *l, struct mlist_elem *start, void *data);


#endif
