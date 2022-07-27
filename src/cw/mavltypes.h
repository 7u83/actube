
/**
 * MAVL AVL Tree type
 */
typedef struct mavl * mavl_t;
typedef struct mavliter mavliter_t;

void * mavl_get_ptr ( mavl_t tree, void * search );


/**
 * Create a mavl_t object which stores pointers. 
 * @param cmp a pointer to the compare function
 * @param del a pointer to a delete function
 * @return The mavl_t object or NULL if an error has occured.
 */
#define mavl_create_ptr(cmp,del)\
	mavl_create(cmp,del,sizeof(void*))
	
int mavl_cmpstr (const void *s1, const void *s2);
#define mavl_create_conststr() mavl_create_ptr(mavl_cmpstr,NULL)

#define mavl_add_str(tree,str) mavl_insert_ptr(tree,str)
#define mavl_get_str(tree,search) ((char *)(mavl_get_ptr(tree,search)))
void * mavl_insert_ptr ( mavl_t tree, const void *ptr );
void *mavl_replace(struct mavl *t,const void  *data, int * replaced);

void * mavl_get_ptr(mavl_t tree, void * search);
void * mavliter_get_ptr(mavliter_t *i);
void mavl_merge(mavl_t target, mavl_t source);

void mavl_freeptr(void *ptr);
#define mavliter_get_str(iter) ((char*)(mavliter_get_ptr(iter)));
#define mavl_get_str(tree,search) ((char *)(mavl_get_ptr(tree,search)))
