#ifndef __CW_TIMER_H
#define __CW_TIMER_H

#include <time.h>

/** 
 * @defgroup TimerFunctions Timer functions
 * @{
 */

/**
 * Start a timer.
 * @param t number of seconds until the timer expires
 * @return timer value to initialize a variable of time_t
 *
 * Example: time_t timer = cw_timer_start(60);
 */
#define cw_timer_start(t) (time(NULL)+t)

/**
 * Check if a timer is expired.
 * @param t an time_t variable intializes by #cw_timer_start
 * @return 0=timer is not expired\n 1=timer is expired.
 */
#define cw_timer_timeout(t) (time(NULL)>t ? 1 : 0)


#define cw_clock_start()\
	clock()

#define cw_clock_stop(t)\
	(((double) (clock()-(t))) / CLOCKS_PER_SEC)

/** @} */


#endif
