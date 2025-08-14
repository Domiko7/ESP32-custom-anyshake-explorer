#include "utils/timer.h"


hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile bool flag = false;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  flag = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}


void initTimer() {
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, GET_INTERVAL * 1000, true);
  timerAlarmEnable(timer);
}