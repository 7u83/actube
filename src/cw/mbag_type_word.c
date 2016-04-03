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

#include "cw.h"
#include "mbag.h"


static struct mbag_item *  from_str(const char *src)
{
	mbag_item_t * i = mbag_item_new(MBAG_WORD);
	if (!i)
		return NULL;
	i->word=atoi(src);
	return i;
}

static int to_str(void *item,char *dst)
{
	mbag_item_t *i= item;
        return sprintf(dst, "%d", i->word);
}

static struct mbag_item * get(const uint8_t *src,int len)
{
	mbag_item_t * item = mbag_item_new(MBAG_WORD);
	if (!item)
		return NULL;
	item->word=cw_get_word(src);
	return item;
}


static int put(struct mbag_item * i, uint8_t *dst)
{
	return cw_put_word(dst,i->word);
}


/** Defines a word, two bytes. */
const struct mbag_typedef mbag_type_word = {
	.name = "Word",
	.del = NULL,
	.to_str = to_str,
	.from_str = from_str,
	.get = get,
	.put = put
};


