#include <errno.h>
#include <stdio.h>
#include "cw/mavl.h"
...

mavldata_t * result, data;
mavl_t tree;

/* create a mavl tree to store integers */

tree = mavl_create(mavl_cmp_dword, NULL);
  
... do some other stuff 

/*  add the value 17 to the tree */
data.dword = 17;
result = mavl_add(tree,&data);

if (result == NULL )
        printf("Error: %s", strerror(errno));
if (result == &data)
        printf("Element already exists\n");
else
        printf("Element successful addded");

}

