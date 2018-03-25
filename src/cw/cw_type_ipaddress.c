
#include <stdio.h>

#include "format.h"
#include "cw.h"
#include "ktv.h"


#include "sock.h"

static void del ( struct cw_KTV * data )
{
	free ( data->val.ptr );
}

static struct cw_KTV *get ( struct cw_KTV * data, const uint8_t * src, int len )
{
	uint8_t * s;
	s = bstr_create ( src, len );
	
	if ( !s )
		return NULL;



	data->type = &cw_type_ipaddress;
	data->val.ptr = s;
	return data;
}

static int put ( const struct cw_KTV *data, uint8_t * dst )
{
	return cw_put_bstr ( dst, data->val.ptr );
}

static int to_str ( const struct cw_KTV *data, char *dst, int max_len )
{

	int l;
	struct sockaddr_storage addr;

	l = bstr_len(data->val.ptr);
	if (l==4){
		addr.ss_family = AF_INET;
		memcpy(&(((struct sockaddr_in*)&addr)->sin_addr),bstr_data(data->val.ptr),4);
		sock_addr2str(&addr,dst);
		return 4;
	}
	if (l==16){
		addr.ss_family = AF_INET6;
		memcpy(&(((struct sockaddr_in6*)&addr)->sin6_addr),bstr_data(data->val.ptr),16);
		sock_addr2str(&addr,dst);
		return 16;
	}
	
	*dst=0;
	return 0;

}

static struct cw_KTV *from_str ( struct cw_KTV * data, const char *src )
{
	struct sockaddr_storage addr;
	uint8_t * s, * addrptr;
	int rc,l;
	
	rc = sock_strtoaddr(src,(struct sockaddr*)&addr);
	if (!rc)
		return NULL;
		
	
	switch(((struct sockaddr*)&addr)->sa_family){
		case AF_INET:
			addrptr = (uint8_t*)&(((struct sockaddr_in*)(&addr))->sin_addr);
			l = 4;
			break;
		case AF_INET6:
			addrptr = (uint8_t*)&(((struct sockaddr_in6*)(&addr))->sin6_addr);
			l = 16;
			break;
		default:
			return NULL;
	}
	

	s = bstr_create(addrptr,l);
	if ( s == NULL )
		return NULL;

	data->type = &cw_type_ipaddress;
	data->val.ptr = s;
	return data;
	
}

static int len ( struct cw_KTV * data ){
	return bstr_len(data->val.ptr);
}


const struct cw_Type cw_type_ipaddress = {
	"IPAddress",		/* name */
	del,			/* del */
	put,			/* put */
	get,			/* get */
	to_str,			/* to_str */
	from_str,		/* from_str */
	len			/* len */
};


