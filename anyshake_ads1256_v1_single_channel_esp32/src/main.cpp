#include <Arduino.h>
#include "settings.h"
#include "ads1256.h"
#include "packet.h"
#include "utils/timer.h"
#include "utils/filter.h"


int32_t reading;
int32_t value; 
uint8_t count = 0;


int32_t channelZ[SAMPLE_COUNT];
int32_t channelE[SAMPLE_COUNT];
int32_t channelN[SAMPLE_COUNT];
uint8_t checksum;

#if ENABLE_COMPENSATION == true
  filter_iir_df1_t zAxisCompensationFilter;
#endif

#if ENABLE_HIGH_PASS == true
  filter_iir_sos_t zAxisPreProcessingHighpassFilter;
#endif

ADS1256 adc(SAMPLE_RATE_BYTES, GAIN_BYTES);

void setup() {
  Serial.begin(115200);

  adc.initADS();

  filter_iir_sos_new(&zAxisPreProcessingHighpassFilter, PRE_PROC_HPF_COEFFS);

  #if ENABLE_COMPENSATION == true
    filter_iir_df1_new(&zAxisCompensationFilter, COMPENSATION_COEFFS_B, COMPENSATION_COEFFS_A);
  #endif

  initTimer();
}

void loop() {
  if (flag) {
    portENTER_CRITICAL(&timerMux);
    flag = false;
    portEXIT_CRITICAL(&timerMux);

    reading = adc.readRaw(); // raw data

    value = reading;
    
    channelZ[count] = value;
    channelE[count] = 0;
    channelN[count] = 0;

    count++;
    //Serial.println(reading);

    if (count == SAMPLE_COUNT) {
      count = 0;
      
      #if ENABLE_HIGH_PASS == true
        apply_data_pre_processing(channelZ, SAMPLE_COUNT, &zAxisPreProcessingHighpassFilter);
      #endif

      #if ENABLE_COMPENSATION == true
        apply_data_compensation(channelZ, SAMPLE_COUNT, &zAxisCompensationFilter);
      #endif

      packAndSendPacket(channelZ, channelE, channelN);
    }
  }
}

