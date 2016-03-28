/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
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
 * @brief Implementation of mavl_create_conststr
 */


#include "mavl.h"

static int cmp(const void *v1,const void*v2)
{
	return strcmp( (char*)v1,(char*)v2);
}

/**
 * Create a mavl object which holds pointers to strings.
 * These strings have to be constant. So there is no 
 * free method called, when the mavl object is destroyed
 * @return The created mavl object or NULL if an error 
 * has occured.
 */

mavl_conststr_t mavl_create_conststr() 
{
	return mavl_create(cmp,NULL);
} 
