
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "format.h"


static struct cw_FormatDumpSettings CW_FORMAT_DUMP_SETTINGS = {
	24,			/* row_len */
	8,			/* marker_distance */
	'|',			/* marker_char */
	1,			/* ascii */
	0,			/* inv_len */
	'*',			/* inv_char */
	"\n\t",			/* dump_prefix */
	"",			/* dump_suffix */
	"  ",			/* ascii_prefix */
	"\n\t"			/* newline */
};

void cw_format_get_dump_defaults(struct cw_FormatDumpSettings * settings)
{
	memcpy(settings, &CW_FORMAT_DUMP_SETTINGS, 
		sizeof(struct cw_FormatDumpSettings));
}


static int cw_format_dump_row(char *dst, const uint8_t * data, int len,
			struct cw_FormatDumpSettings * settings){
	char *pdst, marker;
 	int i;

	pdst = dst;
	for (i = 0; i < len; i++) {
		marker = ((i+1)% settings->marker_distance==0 && i<len-1) 
			? settings->marker_char 
			: ' ';
		pdst += sprintf(pdst, "%02X%c", data[i] & 0xff, marker);
	}
	
	
	if (settings->ascii){
		for(i=len; i<settings->row_len; i++){
			pdst += sprintf(pdst,"   ");
		}

		pdst+=sprintf(pdst,"%s",settings->ascii_prefix);
		for (i = 0; i < len; i++) {
			int c = data[i] & 0xff;
			if (c < 0x20 || c > 0x7f)
				c = '.';
			pdst+=sprintf(pdst,"%c",c);
		}
	}

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
	int row,rows,size;
	char *dst, *pdst;
	
	if (!settings)
		settings = &CW_FORMAT_DUMP_SETTINGS;

	rows = len / settings->row_len;
	if (len % settings->row_len)
		rows++;

	size = strlen(settings->dump_prefix) + strlen(settings->dump_suffix) +
		rows * strlen(settings->newline) + 
		(settings->ascii ? rows*settings->row_len*3 : len * 3);

	if (settings->ascii){
		size += len + rows*strlen(settings->ascii_prefix);
	}
	
	dst = malloc(size+1);
	if (!dst)
		return NULL;

	pdst = dst;
	pdst += sprintf(pdst, "%s",settings->dump_prefix);

	for (row=0; row<rows; row++){
		int rlen, pos;
		pos = row * settings->row_len;
		if (len - pos > settings->row_len){
			rlen = settings->row_len;
		}
		else{
			rlen = len-pos;
		}
		pdst += cw_format_dump_row(pdst,data+pos, rlen, settings);
		if(row<rows-1)
			pdst += sprintf(pdst,"%s",settings->newline);
		else
			pdst += sprintf(pdst,"%s",settings->dump_suffix);			
	}

	return dst;

}

