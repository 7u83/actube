
#include <stdint.h>


extern uint8_t * bstr_create(uint8_t *data, uint8_t len);
extern uint8_t * bstr_create_from_cfgstr(const char * s);
extern uint8_t * bstr_replace( uint8_t ** dst, uint8_t * bstr);


#define bstr_len(s) (*s)
