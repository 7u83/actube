
#include <stdint.h>


extern uint8_t * bstr_create(uint8_t *data, uint8_t len);

#define bstr_len(s) (*s)
