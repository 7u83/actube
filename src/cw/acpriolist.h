#ifndef __ACIPRIORITYLIST_H
#define __ACIPRIORITYLIST_H

#include "mavl.h"
#include "mbag.h"

/**
 * Used to store AC Name With Priority elements.
 */
struct cw_acprio{
	/**
	 * AC Name
	 */
	char *name;
	/**
	 * IP of AC
	 */
	struct sockaddr_storage * addr;
	/**
	 * Priority
	 */
	uint8_t prio;
};

extern  const struct mbag_typedef mbag_type_acprio;

#define MBAG_ACPRIO (&mbag_type_acprio)



typedef struct cw_acprio cw_acprio_t;
typedef struct mavl * cw_acpriolist_t;

extern cw_acpriolist_t cw_acpriolist_create();
cw_acprio_t * cw_acpriolist_add(cw_acpriolist_t l, const char *name,int name_len, uint8_t prio);

static inline int cw_acpriolist_get(cw_acpriolist_t l, char * acname){
	cw_acprio_t ps,*pf;
	ps.name=acname;
	
	pf=mavl_get(l,&ps);
	if (!pf)
		return 256;
	return pf->prio;
}

#define cw_acpriolist_del(l,s) mavl_del(l,s)
#define cw_acpriolist_set(l,n,nl,p) cw_acpriolist_add(l,n,nl,p)

#endif
