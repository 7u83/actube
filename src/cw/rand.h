#ifndef __CW_RAND_H
#define __CW_RAND_H

#include <stdint.h>

int cw_rand(uint8_t*dst, int len);
uint16_t cw_randint(int min,int max);

#endif

