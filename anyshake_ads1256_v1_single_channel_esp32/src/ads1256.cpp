#include "ads1256.h"

ADS1256::ADS1256(uint8_t sps, uint8_t g) {
    sampleRate = sps;
    gain = g;
}

void ADS1256::initADS() {
    spi->begin()
}