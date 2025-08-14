#ifndef __TIMER_H
#define __TIMER_H

#include <Arduino.h>
#include "settings.h"

extern hw_timer_t *timer;
extern portMUX_TYPE timerMux;
extern volatile bool flag;

void IRAM_ATTR onTimer();
void initTimer();

#endif