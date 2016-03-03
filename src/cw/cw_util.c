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


#include <stdio.h>

#include "cw_util.h"
#include "log.h"
#include "capwap.h"

/*
void cw_mand_elem_found(int *l,int type)
{
	if (!cw_dbg_is_level(DBG_CW_RFC))
		return;

	int i;
	for (i=0; l[i]!=-1; i++){
		if(l[i]==type){
			l[i]=0;
			return;
		}
	
	}
}
*/

/*
void cw_get_missing_mand_elems(char *dst, int *l)
{
	if (!cw_dbg_is_level(DBG_CW_RFC))
		return;

	char *s = dst;
	int i;
	const char * k = "";
	for (i=0; l[i]!=-1; i++){
		if(l[i]){
			s += sprintf(s,"%s[%s]",k,cw_strelem(l[i]));
			k=",";
		}
	
	}
}
*/

/*
int cw_is_missing_mand_elems(int *l)
{
	int i;
	for (i=0; l[i]!=-1; i++){
		if(l[i]){
			return 1;
		}
	}
	return 0;
}

*/
