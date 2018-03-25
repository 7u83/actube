/*
    This file is part of actube.

    actube is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    actube is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with actube.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * @file 
 * @brief implents cw_filename
 **/

#include <string.h>
#include <stdio.h>
#include "file.h"

/**
 * @brief Build full pathname from path, filename and extension
 * @param path Path
 * @param name Filename
 * @param ext Extension
 * @return a pointer to the create filename which has to be 
 * released using free.
 */
char * cw_filename(const char * path, const char * name, const char * ext){
	int name_len;
	int ext_len;	
	int size=0;
	
	int path_len;
	char c;
	char * result;
	char * slash="";
	
	path_len = strlen(path);
	if (path_len>0){
		c = path[path_len-1];
		if (!strchr(CW_FILE_PATH_DELIMITERS,c)){
			path_len++;
			slash = "/";
		}
	}
	
	name_len = strlen (name);
	
	

	
	if (ext != NULL){
		if (ext[0]=='.'){
			ext++;
		}
		ext_len = strlen(ext);
	}
	else
		ext_len=-1;

	size = (path_len) + name_len + (ext_len+1) + 1;
	result = malloc(size);
	if (!result)
		return NULL;
		
	strncpy(result,path,path_len);
	result[path_len]=0;
	strcat(result,slash);
	strcat(result,name);
	if (ext){
		strcat(result,".");
		strcat(result,ext);
	}
	
	return result;
}
