#include <stdint.h>
#include "conn.h"
struct dtls_ssl_cert 
{
	int size;
	int type;
	uint8_t * data;
};

extern int dtls_bio_read(struct cw_Conn *conn, char *out, int maxlen);
extern int dtls_bio_write(struct cw_Conn * conn, const char *data, int len);


