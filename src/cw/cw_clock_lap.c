#include "timer.h"

/**
 * Get lap time. (stop watch)
 * @param tv pointer to a clock variable defined with #CW_CLOCK_DEFINE
 * @return lap time in seconds.
 */
double cw_clock_lap(struct timeval *tv)
{
	struct timeval lap;
	gettimeofday(&lap, NULL);
	return (cw_timevaltodouble(&lap) - cw_timevaltodouble(tv)) / 1000000.0;
}
