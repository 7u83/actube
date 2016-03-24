#ifndef __DOT11_H
#define __DOT11_H

#include <stdint.h>
#include <string.h>

extern const uint8_t dot11_tab_br[256];



#define dot11_get_byte(ptr) (*(ptr))
#define dot11_put_byte(ptr,b) (*(ptr) = b)

static inline int dot11_put_word(uint8_t *ptr,uint16_t w)
{
	dot11_put_byte(ptr,w&0xff);
	dot11_put_byte(ptr+1,w>>8);
	return 2;
}

static inline uint16_t dot11_get_word(uint8_t *ptr)
{
	uint16_t w;
	w = dot11_get_byte(ptr+1);
	w|= dot11_get_byte(ptr)<<8;
	return w;
}

#define dot11_get_version(frame) ((frame[1])&0x03)
#define dot11_get_type(frame) (((frame[1])&0x0c) >> 2)
#define dot11_get_subtype(frame) (((frame[1])&0xf0) >> 4)
/** 
 * Get Frame Control field 
 * @param frame
 * @return Frame Control field
 */
#define dot11_get_fc(frame) dot11_get_word(frame)

#define dot11_get_duration(frame) dot11_get_word(frame+2)

static inline void dot11_get_address(uint8_t * dst, uint8_t *frame, int field)
{
	uint8_t *s = frame+4+field*6;
	memcpy(dst,s,6);
}

#define dot11_get_address1(dst,frame) dot11_get_address(dst,frame,0)
#define dot11_get_address2(dst,frame) dot11_get_address(dst,frame,1)
#define dot11_get_address3(dst,frame) dot11_get_address(dst,frame,2)

#define dot11_get_sequence_control(frame) dot11_get_word(frame+22)

#define DOT11_FC_FLAG_FROM_DS  0x100
#define DOT11_FC_FLAG_TO_DS  0x200
#define DOT11_FC_FLAG_MOREFRAGS  0x400
#define DOT11_FC_FLAG_RETRY  0x800
#define DOT11_FC_FLAG_POWERMGMT  0x1000
#define DOT11_FC_FLAG_MOREDATA  0x2000
#define DOT11_FC_FLAG_PROTECTED  0x4000
#define DOT11_FC_FLAG_ORDER  0x8000

#define dot11_fc_get_version(fc) ((fc)&0x3)
#define dot11_fc_get_type(fc) (((fc)&0x0c)>>2)
#define dot11_fc_get_subtype(fc) (((fc)&0xf0)>>4)
#define dot11_fc_get_from_ds(fc) (fc&DOT11_FC_FLAG_FROM_DS?1:0)
#define dot11_fc_get_to_ds(fc) (fc&DOT11_FC_FLAG_TO_DS?1:0)
#define dot11_fc_get_more_fragments(fc) (fc&DOT11_FC_FLAG_MOREFRAGS?1:0)


#endif
