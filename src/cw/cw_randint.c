
#include <stdint.h>


#include "rand.h"

/**
 * @brief Get e random integer in a specific range. 
 * @param min minimum value
 * @param max maximum value
 * @return The random number, which is greater or equal to min and 
 * less than max.
 */
uint16_t cw_randint(int min,int max)
{
	uint16_t randval;
	uint32_t diff;
	
	diff = max-min;
	
	cw_rand((uint8_t*)(&randval),sizeof(randval));
	
	
	return min+diff * randval / 0xffff;
}