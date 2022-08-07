#ifndef _time_h
#define _time_h

#include "driverlib.h"

extern unsigned int time1ms, time2ms ,time5ms, time10ms, time20ms, time100ms, time1s;

void rtc_init(void);
void time_init(void);
#endif
