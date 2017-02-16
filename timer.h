#ifndef TIMER_H
#define TIMER_H

#include <inttypes.h>
#include <sys/time.h>
#include <time.h>

/*
 * https://stackoverflow.com/questions/1861294/
 * how-to-calculate-execution-time-of-a-code-snippet-in-c
 */

uint64_t get_time_ms()
{
	struct timeval tv;
	uint64_t ret;

	gettimeofday(&tv, NULL);
	ret = tv.tv_usec / 1000;
	ret += (tv.tv_sec * 1000);

	return ret;
}
#endif
