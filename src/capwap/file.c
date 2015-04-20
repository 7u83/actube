#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

char *cw_load_file(const char *filename,size_t *size)
{
	FILE * infile = fopen(filename,"rb");
	if ( !infile) 
		return NULL;
	fseek(infile,0,SEEK_END);
	*size = ftell(infile);
	char *buf = malloc(*size);
	if (!buf) 
		goto errX;

	fseek(infile,0,SEEK_SET);
        *size = fread(buf,1,*size,infile);
errX:
	fclose(infile);


	return buf;
}


int cw_save_file(const char *filename, char *data,int len)
{
	FILE *outfile = fopen(filename,"wb");
	if (!outfile)
		return 0;

	int bytes = fwrite(data,1,len,outfile);
	fclose(outfile);
	return bytes;
}
