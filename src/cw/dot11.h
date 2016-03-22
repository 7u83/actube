#ifndef __DOT11_H
#define __DOT11_H

#include <stdint.h>

extern const uint8_t dot11_tab_br[256];

#define dot11_br(b) (dot11_tab_br[b])


#define dot11_get_version(frame) (dot11_br(frame[0])&0x03)
#define dot11_get_type(frame) ((dot11_br(frame[0])&0x0c) >> 2)
#define dot11_get_subtype(frame) ((dot11_br(frame[0])&0x7f) >> 4)

#endif
