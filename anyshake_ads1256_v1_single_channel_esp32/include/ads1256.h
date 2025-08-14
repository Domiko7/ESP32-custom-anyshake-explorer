#ifndef __ADS1256_H
#define __ADS1256_H

#include <SPI.h>
#include <Arduino.h>
#include "settings.h"

class ADS1256 {
    public:
        ADS1256(uint8_t sps, uint8_t g);
        void initADS();
        long readRaw();

    private:
        uint8_t sampleRate;
        uint8_t gain;
        SPIClass *spi;

        void sendCommand(uint8_t cmd);
        void writeRegister(uint8_t reg, uint8_t value);
        uint8_t readRegister(uint8_t reg);
        void waitDRDY();
        void reset();

        
};


#endif
