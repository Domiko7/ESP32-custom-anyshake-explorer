#ifndef __PACKET_H
#define __PACKET_H

#include <Arduino.h>
#include "settings.h"

void getChecksum(int32_t *zSamples, int32_t *eSamples, int32_t *nSamples, uint8_t *calcChecksum);
void packAndSendPacket(int32_t *channelZ, int32_t *channelE, int32_t *channelN);

struct __attribute__((packed)) Packet {
    uint8_t header[2];
    int32_t channelZ[SAMPLE_COUNT];
    int32_t channelE[SAMPLE_COUNT];
    int32_t channelN[SAMPLE_COUNT];
    uint8_t checksum[3];
    uint8_t padding;
};

#endif