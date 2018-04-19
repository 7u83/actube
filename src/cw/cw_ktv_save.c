
#include "ktv.h"


static int write_str(FILE *outfile,const char *str)
{
	if ( (strchr(str,'\\')!=NULL) || (strchr(str,'"')!=NULL) || (strchr(str,'\n')!=NULL)){
		fprintf(outfile,"\"");
		
		while (*str != 0){
			switch(*str){
				case '\n':
					fprintf(outfile,"\\n");
					break;
				case '\\':
					fprintf(outfile,"\\\\");
					break;
				case '"':
					fprintf(outfile,"\\\"");
					break;
				default:
					fprintf(outfile,"%c",*str);
					break;
			}
			str++;
		}

		fprintf(outfile,"\"");
		return 1;
	}
	
	fprintf(outfile,"%s",str);
	return 1;
}

int cw_ktv_save(mavl_t ktvstore, const char * filename){
	mavliter_t it;
	FILE * outfile;
	
	outfile = fopen(filename,"w");
	if (outfile == NULL)
		return 0;
	
	mavliter_init(&it,ktvstore);
	mavliter_foreach(&it){
		cw_KTV_t * val;
		char buf[4000];
		val = mavliter_get(&it);
		val->type->to_str(val,buf,4000);
		write_str(outfile,val->key);
		fprintf(outfile," :%s: ",val->type->name);
		write_str(outfile,buf);
		fprintf(outfile,"\n");
		
	}
	fclose(outfile);
	return 1;
}