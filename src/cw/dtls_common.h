#include <stdint.h>
#include "conn.h"
struct dtls_ssl_cert 
{
	int size;
	int type;
	uint8_t * data;
};

extern int dtls_bio_read(struct conn *conn, char *out, int maxlen);
extern int dtls_bio_write(struct conn * conn, const char *data, int len);


