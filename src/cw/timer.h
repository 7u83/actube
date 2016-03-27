/*
    Tis file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libcapwap.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef __CW_TIMER_H
#define __CW_TIMER_H

#include <time.h>
#include <sys/time.h>

/** 
 * @defgroup TimerFunctions Timer & Clock Functions
 * @{
 */

/** 
 * CAPWAP timer type, used in conjunction with #cw_timer_start and #cw_timer_timeout.
 */
typedef time_t cw_timer_t;


/**
 * Start a timer.
 * @param t number of seconds until the timer expires
 * @return timer value to initialize a variable of #cw_timer_t
 *
 * Example: 
 * @code 
 * cw_timer_t timer = cw_timer_start(60);
 * while (!cw_timer_timeout(timer)){
 *   // Do something for max. 60 seconds ...
 * }
 * @endcode
 */
#define cw_timer_start(t) (time(NULL)+(t))

/**
 * Check if a timer is expired.
 * @param t a variable of type #cw_timer_t intialized by #cw_timer_start
 * @return 0=timer is not expired\n 1=timer is expired.
 */
#define cw_timer_timeout(t) (time(NULL)>(t) ? 1 : 0)

/**
 * Get the number of nano seconds from a timeval.
 */
#define cw_timevaltodouble(tv) \
	(( ((1000000.0)*(double)((tv)->tv_sec) + (double)(tv)->tv_usec)) )

/**
 * Define a clock variable to measure runtime (not CPU runtime, but
 * real time). The created variable can be used with #cw_clock_start 
 * and #cw_clock_lap.
 * @param c name of the variable
 *
 * Example:
 * @code
 * CW_CLOCK_DEFINE(clk);
 * cw_clock_start(clk);
 * // do something ...
 * int t = cw_clock_lap(clk);
 * printf("Caclulation took %d seconds\n",t);
 * @endcode
 * 
 */
#define CW_CLOCK_DEFINE(c)\
	struct timeval c;

/**
 * Start the clock (stop watch), start measuring time
 * @param c a pointer to a variable defined with #CW_CLOCK_DEFINE
 */
#define cw_clock_start(c)\
	gettimeofday(c,NULL);

/**
 * Get lap time. (stop watch)
 * @param tv pointer to a clock variable defined with #CW_CLOCK_DEFINE
 * @return lap time in seconds.
 */
static inline double cw_clock_lap(struct timeval *tv)
{
	struct timeval lap;
	gettimeofday(&lap, NULL);
	return (cw_timevaltodouble(&lap) - cw_timevaltodouble(tv)) / 1000000.0;
}

/**
 * An alias for #cw_clock_lap
 */
#define cw_clock_stop cw_clock_lap

/** @} */


#endif
