#ifndef __DOT11_H
#define __DOT11_H

/**
 * @file
 * @brief Definitions and protoypes for 802.11
 *
 * @defgroup DOT11
 * @{
 */

#include <stdint.h>
#include <string.h>
#include <sys/time.h>


#define DOT11_T_MGM		0b00
#define DOT11_T_CTL		0b01
#define DOT11_T_DTA		0b10
#define DOT11_T_RES		0b11

#define dot11_fc_mgm(subtype) (DOT11_T_MGM << 2 | subtype(<<4))


#define DOT11_FC_ASSOC_REQ	dot11_fc_mgm(0b0000)
#define DOT11_FC_ASSOC_RESP	dot11_fc_mgm(0b0001)
#define DOT11_FC_REASSOC_REQ	dot11_fc_mgm(0b0010)
#define DOT11_FC_REASSOC_RESP	dot11_fc_mgm(0b0011)
#define DOT11_FC_PROBE_REQ	dot11_fc_mgm(0b0100)
#define DOT11_FC_PROBE_RESP	dot11_fc_mgm(0b0101)
#define DOT11_FC_TIMING_ADV	dot11_fc_mgm(0b0110)
#define DOT11_FC_MGM_RES111	dot11_fc_mgm(0b0111)
#define DOT11_FC_BEACON		dot11_fc_mgm(0b1000)






extern const uint8_t dot11_tab_br[256];



#define dot11_get_byte(ptr) (*(ptr))
#define dot11_put_byte(ptr,b) (*(ptr) = b)

static inline int dot11_put_word(uint8_t *ptr,uint16_t w)
{
	dot11_put_byte(ptr+1,w&0xff);
	dot11_put_byte(ptr,w>>8);
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

static inline void dot11_get_address(uint8_t * dst, uint8_t *frame)
{
	memcpy(dst,frame,6);
}

#define dot11_get_address1(dst,frame) dot11_get_address(dst,frame+4)
#define dot11_get_address2(dst,frame) dot11_get_address(dst,frame+4*2*6)
#define dot11_get_address3(dst,frame) dot11_get_address(dst,frame+4*3*6)

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


#define dot11_fc_put_frame_control(dst,fc) dot11_put_word(dst,fc)



extern uint64_t dot11_timer_offset;

static inline uint64_t dot11_timer_get(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return 1000000 * tv.tv_sec + tv.tv_usec - dot11_timer_offset;
}

static inline void dot11_timer_set(uint64_t val) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	dot11_timer_offset = 1000000 * tv.tv_sec + tv.tv_usec - val;
}



/**
 * @}
 */

#endif
