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


#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "acpriolist.h"


static int acprio_cmp(const void *x1, const void *x2)
{
	cw_acprio_t * p1 = (cw_acprio_t*)x1;
	cw_acprio_t * p2 = (cw_acprio_t*)x2;
	return strcmp (p1->name,p2->name);

}

static void acprio_del(void *d)
{
	cw_acprio_t *p = (cw_acprio_t*)d;
	if ( p->name ) 
		free (p->name);
//	if ( p->addr )
//		free(p->addr);
	free(d);
}

const struct mbag_typedef mbag_type_acprio = {
	"acprio",acprio_del
};




cw_acpriolist_t cw_acpriolist_create()
{
	return  mavl_create(acprio_cmp, acprio_del);
}

cw_acprio_t * cw_acpriolist_add(cw_acpriolist_t l, const char *name,int name_len, uint8_t prio)
{
		
	cw_acprio_t * s=malloc(sizeof(cw_acprio_t));
	if (!s)
		return 0;
	s->name=strndup(name,name_len);
	s->prio=prio;

	cw_acpriolist_del(l,s);
	return mavl_add(l,s);
	
}


 
