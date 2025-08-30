#ifndef __SETTINGS_H
#define __SETTINGS_H

#define ENABLE_COMPENSATION true
#define ENABLE_HIGH_PASS true

#define SAMPLE_RATE 100 
#define SAMPLE_COUNT 5
#define GAIN 64

#define GAIN_BYTES 0x26  // 00100110
#define SAMPLE_RATE_BYTES B10000010

#define SEND_INTERVAL ((5 * 1000) / SAMPLE_RATE)
#define GET_INTERVAL (SEND_INTERVAL / SAMPLE_COUNT)

#define PACKET_SIZE 66

#define CLOCKMHZ 7.68
#define VREF 2.5

#define PIN_SCK 18
#define PIN_MISO 19
#define PIN_MOSI 23
#define PIN_CS 5
#define PIN_DRDY 16
#define PIN_RST -1
#define PIN_SYNC -1

#endif