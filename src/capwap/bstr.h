#ifndef __BSTR_H
#define __BSTR_H


#include <stdint.h>

typedef uint8_t* bstr_t;

extern uint8_t * bstr_create(uint8_t *data, uint8_t len);
extern uint8_t * bstr_create_from_cfgstr(const char * s);
extern uint8_t * bstr_replace( uint8_t ** dst, uint8_t * bstr);


#define bstr_len(s) (*(s))
#define bstr_data(s) (s+1)


#endif

