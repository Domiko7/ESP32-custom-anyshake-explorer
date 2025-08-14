#ifndef __SETTINGS_H
#define __SETTINGS_H

#define ENABLE_COMPENSATION true

#define SAMPLE_RATE 100 // If you're using the filter for 4.5hz geophones 100 is mandatory if you're not make sure your value can be divided by 2 (only 100 sps for now)
#define SAMPLE_COUNT 5
#define GAIN 64

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
#define PIN_RESET -1
#define PIN_SYNC -1


#endif