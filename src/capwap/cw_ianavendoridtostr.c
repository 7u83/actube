
#include "capwap.h"

/* definition of some iana assigned vendor id's */
const char * cw_ianavendoridtostr(int id){
	switch(id){
		case 890:
			return "ZyXEL Communications Corp.";
		case 11591:
			return "Free Software Foundation";
		case 4232704:
			return "Cisco";
		default:
			return "Unknown";
	}
}
