#include <stdio.h>

#include "capwap80211_types.h"
#include "dot11.h"





static int to_str(void *item,char *dst)
{
	mbag_item_t *it= item;

	uint8_t *data = (uint8_t*)it->data;
	int n=*data;
	data++;

	char *d=dst;
	char *space="";
	int i;
	for (i=0; i<n; i++){
		int val = data[i];


		d+=sprintf(d,"%s",space);
		if (val & 0x80){
			d+=sprintf(d,"*");
		}
		
		d+=sprintf(d,"%0.1f",dot11_rate2float(val & 0x7f));

		space=" ";

	}

	return d-dst;	
}



static struct mbag_item *  from_str(const char *src)
{
	mbag_item_t * item = mbag_item_new(CAPWAP80211_TYPE_RATESET);
	if (!item)
		return NULL;

	if (strlen(src)==0)
		return 0;

	uint8_t rates[64];
	int nrates =0;

	const char *s = src;

	while (*s!=0){
		while (*s==' ') 
			s++;
		int m=0;
		if(*s=='*'){
			m=0x80;
			s++;
		}
		else{
			m=0;
		}
	
		float val;
		int n=sscanf(s,"%f",&val);
		if (n!=1)
			break;
		
		int r = dot11_float2rate(val) | m;

		rates[nrates++]=r;


		while (*s!=0 && *s!=' ')
			s++;

	}

	uint8_t *data = malloc(nrates+1);
	*data=nrates;
	memcpy(data+1,rates,nrates);

	item->data=data;


	return item;
}

static struct mbag_item *  get(const uint8_t *src,int len)
{

	mbag_item_t * item = mbag_item_new(MBAG_BIN);
	if (!item)
		return NULL;

	uint8_t *data = malloc(len+1);
	if (!data){
		free (item);
		return NULL;
	}
	*data=len;
	memcpy(data+1,src,len);
	item->data=data;
	return item;
}


const struct mbag_typedef capwap80211_type_rateset = {
	.name = "Biary",
	.del = free,
	.from_str = from_str,
	.to_str = to_str,
	.get = get

};
