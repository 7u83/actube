
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "format.h"


static struct cw_FormatDumpSettings CW_FORMAT_DUMP_SETTINGS = {
	32/*CW_FORMAT_DUMP_ROW_LEN*/,	/* rowlen */
	1,				/* ascii */
	0,				/* settings->invlen */
	"\n\t",				/* dump_prefix */
};




static int cw_format_dump_row(char *dst, const uint8_t * data, int len){
	char *pdst = dst;
	int i;
	char *sp;
	for (i = 0; i < len; i++) {
		sp = ((i+1)%4==0 && i<len-1) ? "|" : " ";
		pdst += sprintf(pdst, "%02X%s", data[i] & 0xff, sp);
	}
	
	pdst+=sprintf(pdst,"   ");
	
	for (i = 0; i < len; i++) {
		int c = data[i] & 0xff;
		if (c < 0x20 || c > 0x7f)
			c = '.';
		pdst+=sprintf(pdst,"%c",c);
	}

	pdst+=sprintf(pdst,"%s","\n");
	return pdst-dst;
}



/**
 * Create an ASCII hex dump of binary data
 * 
 * @param data data to dump
 * @param len number of bytes to dump (size of data)
 * @return a character string with the created data ASCII 
 * dump (must be released with free).If memory cannot be allocated
 * the return value is NULL.
 */
char *cw_format_dump(const uint8_t * data, int len, 
	struct cw_FormatDumpSettings *settings)
{
	int i;

	int row,rows;
	

	

	if (!settings)
		settings = &CW_FORMAT_DUMP_SETTINGS;

	rows = len / settings->rowlen;
	
	printf("Number fo rows: %d\n",rows);

	int md;
	if (settings->ascii)
		md = 2;
	else
		md = 1;

	char *dst = malloc(2 * (md * (len * 3 + (rows * 2) + 8 )));
	if (!dst)
		return NULL;

	if (len % settings->rowlen)
		rows++;

	char *pdst = dst;
	
	pdst += sprintf(pdst, "%s",settings->dump_prefix);
/*	pdst += 2; */

	char asc_buffer[128];
	char *ascdst = asc_buffer;

	pdst = dst;
	for (row; row<rows; row++){
		int n;
		pdst += cw_format_dump_row(pdst,data+row*settings->rowlen,settings->rowlen);
	}
	return dst;


	for (i = 0; i < len; i++) {
		char *sp = " ";
		if (i == settings->invlen - 1)
			sp = "|";

		pdst += sprintf(pdst, "%02X%s", data[i] & 0xff, sp);
		if (settings->ascii) {
			int c = data[i] & 0xff;
			if (c < 0x20 || c > 0x7f)
				c = '.';
			*ascdst = c;
			ascdst++;
		}

		if ((i + 1) % settings->rowlen == 0) {
			int l;
			if (settings->ascii) {
				*ascdst = 0;
				l = sprintf(pdst, " | %s\n\t", asc_buffer);
				ascdst = asc_buffer;

			} else {
				l = sprintf(pdst, "\n\t");
			}
			pdst += l;
		}

	}

	if (settings->ascii) {
		*ascdst = 0;
		if (strlen(asc_buffer))
			pdst += sprintf(pdst, " | %s", asc_buffer);
	}

	return dst;
}

