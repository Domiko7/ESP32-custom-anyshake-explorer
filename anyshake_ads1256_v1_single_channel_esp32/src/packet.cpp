#include "packet.h"


void getChecksum(int32_t *zSamples, int32_t *eSamples, int32_t *nSamples, uint8_t *calcChecksum) {
    uint8_t *bytes;

    bytes = (uint8_t*) zSamples;
    for (uint8_t i = 0; i < SAMPLE_COUNT * sizeof(int32_t); i++) {
        calcChecksum[0] ^= bytes[i];
    }

    bytes = (uint8_t*) eSamples;
    for (uint8_t i = 0; i < SAMPLE_COUNT * sizeof(int32_t); i++) {
        calcChecksum[1] ^= bytes[i];
    }

    bytes = (uint8_t*) nSamples;
    for (uint8_t i = 0; i < SAMPLE_COUNT * sizeof(int32_t); i++) {
        calcChecksum[2] ^= bytes[i];
    }
}

void packAndSendPacket(int32_t *channelZ, int32_t *channelE, int32_t *channelN) {
    
    Packet packet;

    // HEADER
    packet.header[0] = 0xFC;
    packet.header[1] = 0x1B;

    // DATA
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        packet.channelZ[i] = channelZ[i];
        packet.channelE[i] = channelE[i];
        packet.channelN[i] = channelN[i];
    }

    // CHECKSUM
    uint8_t calcChecksum[3] = {0, 0, 0};
    getChecksum(channelZ, channelE, channelN, calcChecksum);
    packet.checksum[0] = calcChecksum[0];
    packet.checksum[1] = calcChecksum[1];
    packet.checksum[2] = calcChecksum[2];

    // PADDING
    packet.padding = 0x00;

    Serial.write(reinterpret_cast<uint8_t*>(&packet), sizeof(Packet));

}