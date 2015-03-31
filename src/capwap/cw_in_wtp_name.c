


int cw_in_wtp_name(struct conn *conn,struct cw_action * a,uint8_t *data,int len)
{
	int i;

	for (i=0; i<len; i++) {
		printf("%c",data[i]);
	}
	printf("\n");
}


