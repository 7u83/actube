#include "dot11.h"


uint16_t dot11_get_word(uint8_t * ptr)
{
	uint16_t w;
	w = dot11_get_byte(ptr + 1);
	w |= dot11_get_byte(ptr) << 8;
	return w;
}

void dot11_get_address(uint8_t * dst, uint8_t * frame)
{
	memcpy(dst, frame, 6);
}

int dot11_put_frame_control(uint8_t * dst, int fctype, int flags)
{
	uint16_t fc = fctype | flags;
	return dot11_fc_put_frame_control(dst, fc);
}

/**
 * Get the current timestamp
 * @return current timestamp in microseconds
 */
uint64_t dot11_timer_get()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return 1000000 * tv.tv_sec + tv.tv_usec - dot11_timer_offset;
}



void dot11_timer_set(uint64_t val)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	dot11_timer_offset = 1000000 * tv.tv_sec + tv.tv_usec - val;
}

int  dot11_put_ssid(uint8_t *dst,uint8_t * ssid,int len){
	dot11_put_byte(dst,DOT11_ELEM_SSID);
	dot11_put_byte(dst+1,len);
	memcpy(dst+2,ssid,len);
	return len;

}

int dot11_put_supported_rates(uint8_t *dst, float *basic, float *rates){
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


int dot11_put_dsss_param_set(uint8_t *dst,int ch) {

	dot11_put_byte(dst,DOT11_ELEM_DSSS_PARAM_SET);
	dot11_put_byte(dst+1,1);
	dot11_put_byte(dst+2,ch);
	return 3;
}


