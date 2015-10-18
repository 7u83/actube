#ifndef __CW_FILE_H
#define __CW_FILE_H

#include <stdlib.h>

/**
 *@defgroup FileFunctions File Functions
 *@{
 */

char *cw_load_file(const char *filename,size_t *size);
int cw_save_file(const char *filename, char *data,int len);

/**@}*/

#endif


