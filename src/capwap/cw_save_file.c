#include <stdio.h>

#include "file.h"


int cw_save_file(const char *filename, char *data, int len)
{
	FILE *outfile = fopen(filename, "wb");
	if (!outfile)
		return 0;

	int bytes = fwrite(data, 1, len, outfile);
	fclose(outfile);
	return bytes;
}
