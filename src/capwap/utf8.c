#include "capwap.h"

int cw_is_utf8(unsigned char *str, size_t len)
{
	size_t i = 0;
	size_t bytes = 0;

	while (i < len) {
		if (str[i] < 0x20) {
			return 0;
		}
		if (str[i] <= 0x7F)
			bytes = 0;
		else if (str[i] >= 0xC0 /*11000000 */  && str[i] <= 0xDF /*11011111 */ )
			bytes = 1;
		else if (str[i] >= 0xE0 /*11100000 */  && str[i] <= 0xEF /*11101111 */ )
			bytes = 2;
		else if (str[i] >= 0xF0 /*11110000 */ 
			 && str[i] <= 0xF4 /* Cause of RFC 3629 */ )
			bytes = 3;
		else {
			return 0;
		}
		i += 1;
		while (i < len && bytes > 0 && str[i] >= 0x80 && str[i] <= 0xBF) {
			i += 1;
			bytes -= 1;
		}
		if (bytes != 0) {
			return 0;
		}
	}
	return 1;
}
