#include <Arduino.h>
#include "settings.h"
#include "packet.h"
#include "utils/timer.h"


long reading;
int32_t value; 
uint8_t count = 0;


int32_t channelZ[SAMPLE_COUNT];
int32_t channelE[SAMPLE_COUNT];
int32_t channelN[SAMPLE_COUNT];
uint8_t checksum;

void setup() {
  Serial.begin(115200); 

  initTimer();
}

void loop() {
  if (flag) {
    portENTER_CRITICAL(&timerMux);
    flag = false;
    portEXIT_CRITICAL(&timerMux);

    reading = 0.0; // raw data
    
    value = (int32_t)reading;
    channelZ[count] = value;
    channelE[count] = 0;
    channelN[count] = 0;

    count++;
    Serial.println(reading);

    if (count == SAMPLE_COUNT) {
      count = 0;
      //packAndSendPacket(channelZ, channelE, channelN);
    }
  }
}

