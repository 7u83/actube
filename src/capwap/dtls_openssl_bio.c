#include <errno.h>
#include <arpa/inet.h>
#include "dtls_openssl.h"

#include "conn.h"

int dtls_openssl_bio_write(BIO *b, const char *data, int len)
{
	printf("bio_write %p %d",data,len);

	struct conn * conn = b->ptr;
	uint8_t buffer[2048];
	*((uint32_t*)buffer)=htonl(1<<24);
	memcpy(buffer+4,data,len);
	int rc = conn->send_packet(conn,buffer,len+4);

	printf("Bio wr rc = %d\n",rc);


	if (rc<0)
		return rc;
	return rc-4;
}


int dtls_openssl_bio_read(BIO *b, char *out, int maxlen)
{
	struct conn * conn = b->ptr;
	struct dtls_openssl_data * dtls_data = conn->dtls_data;
	if (dtls_data->len==0){
		int len  = conn->recv_packet(conn,dtls_data->buffer,2048);
/*		if (len == -1){
			printf ("-1 ERRRRRRRRRRRRRRRRR %s\n",strerror(errno));
		}
*/
		if (len<4)
			return 0;
		dtls_data->len=len-4;
		dtls_data->pos=4;
	}

	if (dtls_data->len > maxlen)
	{
		memcpy(out,dtls_data->buffer+dtls_data->pos,maxlen);
		dtls_data->len-=maxlen;
		dtls_data->pos+=maxlen;
		return maxlen;

	}
	memcpy(out,dtls_data->buffer+dtls_data->pos,dtls_data->len);
	int ret = dtls_data->len;
	dtls_data->len=0;
	return ret;
}



int dtls_openssl_bio_new(BIO *bi)
{
	bi->init = 1;
	bi->num = 0;
	bi->flags = 0;
	bi->ptr = NULL; 
	return 1;
}



int dtls_openssl_bio_puts(BIO *b, const char *str)
{
//	printf("Bio puts: %s\n",str);
	return dtls_openssl_bio_write(b, str, strlen(str));
}

int dtls_openssl_bio_free(BIO *bio)
{
	if (bio == NULL) 
		return 0;
	return 1;
}



long dtls_openssl_bio_ctrl(BIO *b, int cmd, long num, void *ptr)
{
	long ret = 1;
//	long sockopt_val = 0;
//	unsigned int sockopt_len = 0;
//	BIO_memory_data* pData = (BIO_memory_data*)b->ptr;
//
//	printf("BIO CONTROL cmd=%d,num=%d\n",cmd,num);
//	exit(1);	

	switch (cmd)
	{
		case BIO_CTRL_RESET:
		case BIO_CTRL_EOF:
			ret = 0;
			break;

		case BIO_CTRL_INFO:
			ret = 0;
			break;

		case BIO_CTRL_GET_CLOSE:
			ret = 0;
			break;

		case BIO_CTRL_SET_CLOSE:
			break;

		case BIO_CTRL_WPENDING:
			ret = 0;
			break;
	
		case BIO_CTRL_PENDING:
			ret = 0;
			break;

		case BIO_CTRL_DUP:
			ret = 1;
			break;

		case BIO_CTRL_FLUSH:
			ret = 1;
			break;

		case BIO_CTRL_PUSH:
			ret = 0;
			break;

		case BIO_CTRL_POP:
			ret = 0;

		case BIO_CTRL_DGRAM_QUERY_MTU:
		{
			ret = 1300;
			break;

/*         	sockopt_len = sizeof(sockopt_val);
			if ((ret = getsockopt(pData->sock, IPPROTO_IP, IP_MTU, (void *)&sockopt_val, &sockopt_len)) < 0 || sockopt_val < 0)
			{ 
				ret = 0; 
			}
			else
			{
				pData->nMtu = sockopt_val;
				ret = sockopt_val;
			}

			break;	

*/		}

		case BIO_CTRL_DGRAM_GET_MTU:
			ret = 1500;
//			ret = pData->nMtu;
			break;
	
		case BIO_CTRL_DGRAM_SET_MTU:
//			pData->nMtu = num;
//			ret = num;
			break;

		default:
			ret = 0;
			break;
	}

	return ret;
}


