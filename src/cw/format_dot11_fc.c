#include "dot11.h"
#include "format.h"


static inline int  add_flag(char *dst,int fc, int flag, char *name,char **delim)
{
	if (! (fc&flag) )
		return 0;
	int n = sprintf(dst,"%s%s",*delim,name);
	*delim=":";
	return n;
}

int format_dot11_fc(char *dst, uint16_t fc)
{
	char *d = dst;

	d+=sprintf(d,"Ver: %d ",dot11_fc_get_version(fc));
	d+=sprintf(d,"Type: %d ",dot11_fc_get_type(fc));
	d+=sprintf(d,"Subtype: %d ",dot11_fc_get_subtype(fc));
	d+=sprintf(d,"Flags: [");

	char *delim = "";

	d+=add_flag(d,fc,DOT11_FC_FLAG_FROM_DS,"from_ds",&delim);
	d+=add_flag(d,fc,DOT11_FC_FLAG_TO_DS,"to_ds",&delim);
	d+=add_flag(d,fc,DOT11_FC_FLAG_MOREFRAGS,"more_frags",&delim);
	d+=add_flag(d,fc,DOT11_FC_FLAG_RETRY,"retry",&delim);
	d+=add_flag(d,fc,DOT11_FC_FLAG_POWERMGMT,"pwrmgmt",&delim);
	d+=add_flag(d,fc,DOT11_FC_FLAG_MOREDATA,"more data",&delim);
	d+=add_flag(d,fc,DOT11_FC_FLAG_PROTECTED,"protected",&delim);
	d+=add_flag(d,fc,DOT11_FC_FLAG_ORDER,"order",&delim);

	d+=sprintf(d,"]");
	return d-dst;

}




