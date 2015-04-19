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

#ifndef __AVLTREE_H
#define __AVLTREE_H

#include "mavl.h"

#define avltree mavl
#define avltree_get mavl_get
#define avltree_destroy mavl_destroy
#define avltree_create mavl_create
#define avltree_add mavl_add
#define avltree_del mavl_del
#define avltree_replace mavl_replace
#define avliter_foreach_from mavliter_foreach_from
#define avliter_foreach mavliter_foreach
#define avltree_foreach_asc mavl_foreach_asc
#define avliter_get mavliter_get
#define avltree_replace_data mavl_replace_data
#define avliter_seek mavliter_seek
#define avliter_seek_set mavliter_seek_set
#define avliter_next mavliter_next
#define avltree_del_all mavl_del_all

#define DEFINE_AVLITER MAVLITER_DEFINE

#endif


