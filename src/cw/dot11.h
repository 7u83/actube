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

#ifdef __FreeBSD__
#include <sys/ednian.h>
#else
#include <endian.h>
#endif


/**
 * @defgroup DOT11_FRAME_TYPES Frame Types
 * @brief Definition of frame types
 * 
 * @{
 */
/** Management Frame */
#define DOT11_T_MGM		0b00
/** Control Frame */
#define DOT11_T_CTL		0b01
/** Data Frame **/
#define DOT11_T_DTA		0b10
/** Reserved Frame Type */
#define DOT11_T_RES		0b11
/**
 * @}
 */

#define dot11_fc_mgm(subtype) ((DOT11_T_MGM << 2) |( subtype<<4))
#define dot11_fc_dta(subtype) ((DOT11_T_DTA << 2) |( subtype<<4))

/** 
 * @defgroup DOT11_FRAMES Frame Controls
 *
 * @{
 */
#define DOT11_FC_ASSOC_REQ	dot11_fc_mgm(0b0000)
#define DOT11_FC_ASSOC_RESP	dot11_fc_mgm(0b0001)
#define DOT11_FC_REASSOC_REQ	dot11_fc_mgm(0b0010)
#define DOT11_FC_REASSOC_RESP	dot11_fc_mgm(0b0011)
#define DOT11_FC_PROBE_REQ	dot11_fc_mgm(0b0100)
#define DOT11_FC_PROBE_RESP	dot11_fc_mgm(0b0101)
#define DOT11_FC_TIMING_ADV	dot11_fc_mgm(0b0110)
#define DOT11_FC_MGM_RES111	dot11_fc_mgm(0b0111)
#define DOT11_FC_BEACON		dot11_fc_mgm(0b1000)

#define DOT11_FC_DATA		dot11_fc_dta(0b0000)

/**
 * @}
 */


/**
 * @defgroup DOT11_FC_FLAGS Frame Control Flags
 * @{
 */
#define DOT11_FC_FLAG_TO_DS	0x100
#define DOT11_FC_FLAG_FROM_DS	0x200
#define DOT11_FC_FLAG_MOREFRAGS	0x400
#define DOT11_FC_FLAG_RETRY	0x800
#define DOT11_FC_FLAG_POWERMGMT	0x1000
#define DOT11_FC_FLAG_MOREDATA  0x2000
#define DOT11_FC_FLAG_PROTECTED	0x4000
#define DOT11_FC_FLAG_ORDER	0x8000
/**
 * @}
 */

/**
 * @defgroup DOT11_CAP_FLAGS Capability Flags
 * @brief Flags used in the capability field.
 * @{
 */
#define DOT11_CAP_ESS			0x0001
#define DOT11_CAP_IBSS			0x0002
#define DOT11_CAP_CF_POLLABLE		0x0004
#define DOT11_CAP_CF_POLL_REQ		0x0008
#define DOT11_CAP_PRIVACY		0x0010
#define DOT11_CAP_SHORT_PREAMBLE	0x0020
#define DOT11_CAP_PBCC			0x0040
#define DOT11_CAP_CHANNEL_AGILITY	0x0080
#define DOT11_CAP_SPEC_MGMT		0x0100
#define DOT11_CAP_QOS			0x0200
#define DOT11_CAP_SHORT_SLOT_TIME	0x0400
#define DOT11_CAP_APSD			0x0800
#define DOT11_CAP_RADIO_MEASURE		0x1000
#define DOT11_CAP_DSSS_ODFM		0x2000
#define DOT11_CAP_DELAYED_BLOCK_ACK	0x4000
#define DOT11_CAP_IMEDIATE_BLOCK_ACK	0x8000
/**
 * @}
 */

/**
 * @defgroup DOT11_ELEMS 
 * @{
 */
#define DOT11_ELEM_SSID			0
#define DOT11_ELEM_SUPPORTED_RATES	1
#define DOT11_ELEM_FH_PARAM_SET		2
#define DOT11_ELEM_DSSS_PARAM_SET	3
#define DOT11_ELEM_CF_PARAM_SET		4
/**
 * @}
 */


extern const uint8_t dot11_tab_br[256];



#define dot11_get_byte(ptr) (*(ptr))

#define dot11_put_byte(ptr,b) (*(ptr) = b)
#define dot11_put_word(dst,v) ((*((uint16_t*)(dst))=htobe16(v)),2)
#define dot11_put_dword(dst,v) ((*((uint16_t*)(dst))=htobe16(v)),4)
#define dot11_put_qword(dst,v) ((*((uint64_t*)(dst))=htobe64(v)),8)


static inline uint16_t dot11_get_word(uint8_t * ptr)
{
	uint16_t w;
	w = dot11_get_byte(ptr + 1);
	w |= dot11_get_byte(ptr) << 8;
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

static inline void dot11_get_address(uint8_t * dst, uint8_t * frame)
{
	memcpy(dst, frame, 6);
}

#define dot11_get_address1(dst,frame) dot11_get_address(dst,frame+4)
#define dot11_get_address2(dst,frame) dot11_get_address(dst,frame+4+1*6)
#define dot11_get_address3(dst,frame) dot11_get_address(dst,frame+4+2*6)

#define dot11_get_sequence_control(frame) dot11_get_word(frame+22)






#define dot11_fc_get_version(fc) ((fc)&0x3)
#define dot11_fc_get_type(fc) (((fc)&0x0c)>>2)
#define dot11_fc_get_subtype(fc) (((fc)&0xf0)>>4)
#define dot11_fc_get_from_ds(fc) (fc&DOT11_FC_FLAG_FROM_DS?1:0)
#define dot11_fc_get_to_ds(fc) (fc&DOT11_FC_FLAG_TO_DS?1:0)
#define dot11_fc_get_more_fragments(fc) (fc&DOT11_FC_FLAG_MOREFRAGS?1:0)


#define dot11_fc_put_frame_control(dst,fc) dot11_put_word(dst,fc)

static inline int dot11_put_frame_control(uint8_t * dst, int fctype, int flags)
{
	uint16_t fc = fctype | flags;
	return dot11_fc_put_frame_control(dst, fc);
}


/**
 * @defgroup DOT11_TIMER Timer
 * @{
 */
extern uint64_t dot11_timer_offset;

/**
 * Get the current timestamp
 * @return current timestamp in microseconds
 */
static inline uint64_t dot11_timer_get()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return 1000000 * tv.tv_sec + tv.tv_usec - dot11_timer_offset;
}



static inline void dot11_timer_set(uint64_t val)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	dot11_timer_offset = 1000000 * tv.tv_sec + tv.tv_usec - val;
}

/**
 * @}
 */

/**
 * Put a timestamp field
 * @param dst Destination 
 * @return number of bytes
 *
 * The timestamp value is taken from the value returned by #dot11_timer_get.
 */
#define dot11_put_timestamp(dst) (dot11_put_qword(dst,dot11_timer_get()))
#define dot11_put_beacon_interval(dst,v) (dot11_put_word(dst,v))
#define dot11_put_duration(dst,v) (dot11_put_word(dst,v))
#define dot11_put_address(dst,addr) (memcpy(dst,addr,6),6)
#define dot11_put_sequence_control(dst,v) (dot11_put_word(dst,v))
#define dot11_put_capability(dst,v) dot11_put_word(dst,v)
static inline int  dot11_put_ssid(uint8_t *dst,uint8_t * ssid,int len){
	dot11_put_byte(dst,DOT11_ELEM_SSID);
	dot11_put_byte(dst+1,len);
	memcpy(dst+2,ssid,len);
	return len;

}

#define dot11_float2rate(frate) ((int)(((frate)+0.49)*2.0))
#define dot11_rate2float(rate) (((float)(rate))/2.0)

static inline int dot11_put_supported_rates(uint8_t *dst, float *basic, float *rates){
	uint8_t *d = dst+2;
	while(*basic != 0.0){
		*d++ = 0x80 | dot11_float2rate(*basic);
		basic++;
	}
	while(*rates != 0.0){
		*d++ = dot11_float2rate(*rates);
		rates++;
	}
	*(dst)=DOT11_ELEM_SUPPORTED_RATES;
	*(dst+1) = d-(dst+2);

	return d-dst;
}


static inline int dot11_put_dsss_param_set(uint8_t *dst,int ch) {

	dot11_put_byte(dst,DOT11_ELEM_DSSS_PARAM_SET);
	dot11_put_byte(dst+1,1);
	dot11_put_byte(dst+2,ch);
	return 3;
}

extern int dot11_create_beacon(uint8_t *dst);


extern uint8_t dot11_broadcast_address[6];
#define DOT11_BROADCAST_ADDRESS (dot11_broadcast_address)






/**
 * @}
 */

#endif
