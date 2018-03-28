#include "cw.h"
#include "log.h"


int cw_read_from(struct conn *conn, struct sockaddr_storage * from)
{
	int n;
	uint8_t buf[2024];
	int len = 2024;
	
	if (!conn->readfrom) {
		cw_log(LOG_ERR, "Fatal error, no readfrom method available.");
		errno = EPROTO;
		return -1;
	}



	
	n = conn->readfrom(conn, buf, len, from);
	if (n < 0)
		return n;

	if (n > 0) {
		return conn->process_packet(conn, buf, n, (struct sockaddr *) from);
	}
	errno = EAGAIN;
	return -1;
}
