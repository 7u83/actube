#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @file
 */

/**
 * Load a file from disk to memory
 * @param filename name of file to load
 * @param size variable which receives the size of the file in bytes
 * @return a pointer to the memory where the file was loaded in \n 
 * The memory must be freed using free.
 *
 * Eexample:
 * \code
   #include "capwap/file.h"
   size_t bytes;
   char * data = cw_load_file("file.txt",&bytes);
   if (data)
	free (data);
   \endcode
 */ 
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
