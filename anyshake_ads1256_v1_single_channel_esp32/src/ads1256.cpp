#include "ads1256.h"
#include <Arduino.h>

volatile bool DRDY_flag = false;

// Interrupt handler for DRDY pin
void IRAM_ATTR DRDY_ISR() {
    DRDY_flag = true;
}

ADS1256::ADS1256(uint8_t sps, uint8_t g) {
    sampleRate = sps;
    gain = g;
    spi = &SPI;
}

void ADS1256::initADS() {
    pinMode(PIN_DRDY, INPUT);
    pinMode(PIN_CS, OUTPUT);
    digitalWrite(PIN_CS, HIGH);

    if (PIN_RST >= 0) {
        pinMode(PIN_RST, OUTPUT);
        digitalWrite(PIN_RST, HIGH);
    }

    attachInterrupt(digitalPinToInterrupt(PIN_DRDY), DRDY_ISR, FALLING);

    spi->begin();

    // Hardware reset
    if (PIN_RST >= 0) {
        digitalWrite(PIN_RST, LOW);
        delay(10);
        digitalWrite(PIN_RST, HIGH);
        delay(100);
    }

    reset();
    delay(100);

    // Configure registers using raw addresses
    writeRegister(0x01, 0x01);         // MUX register, example value
    writeRegister(0x02, gain);         // ADCON register, set gain
    writeRegister(0x03, sampleRate);   // DRATE register, sample rate
    delay(50);

    sendCommand(0xF0); // Self-calibration
    delay(100);
}

// Wait for DRDY interrupt
void ADS1256::waitDRDY() {
    while (!DRDY_flag) { }
    noInterrupts();
    DRDY_flag = false;
    interrupts();
}

int32_t ADS1256::readRaw() {
    waitDRDY();

    spi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
    digitalWrite(PIN_CS, LOW);

    spi->transfer(0x01); // RDATA command
    delayMicroseconds(7);

    uint8_t buf[3];
    buf[0] = spi->transfer(0x00);
    buf[1] = spi->transfer(0x00);
    buf[2] = spi->transfer(0x00);

    digitalWrite(PIN_CS, HIGH);
    spi->endTransaction();

    int32_t result = 0;
    result |= buf[0];
    result <<= 8;
    result |= buf[1];
    result <<= 8;
    result |= buf[2];

    // Sign conversion for 24-bit
    if (result & 0x800000) {
        result -= 16777216; // 2^24
    }

    return result;
}

void ADS1256::sendCommand(uint8_t cmd) {
    waitDRDY();
    spi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
    digitalWrite(PIN_CS, LOW);
    spi->transfer(cmd);
    delayMicroseconds(10);
    digitalWrite(PIN_CS, HIGH);
    spi->endTransaction();
}

void ADS1256::writeRegister(uint8_t reg, uint8_t value) {
    uint8_t prev = readRegister(reg);
    if (prev != value) {
        waitDRDY();
        spi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
        digitalWrite(PIN_CS, LOW);
        spi->transfer(0x50 | reg); // WREG command + reg
        spi->transfer(0x00);       // write only 1 register
        spi->transfer(value);
        delayMicroseconds(10);
        digitalWrite(PIN_CS, HIGH);
        spi->endTransaction();
    }
}

uint8_t ADS1256::readRegister(uint8_t reg) {
    waitDRDY();
    spi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
    digitalWrite(PIN_CS, LOW);
    spi->transfer(0x10 | reg); // RREG command + reg
    spi->transfer(0x00);       // read only 1 register
    uint8_t value = spi->transfer(0x00);
    delayMicroseconds(10);
    digitalWrite(PIN_CS, HIGH);
    spi->endTransaction();
    return value;
}

void ADS1256::reset() {
    spi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
    digitalWrite(PIN_CS, LOW);
    spi->transfer(0xFE);   // RESET command
    delay(2);
    spi->transfer(0x0F);   // SDATAC command (stop continuous read)
    delayMicroseconds(100);
    digitalWrite(PIN_CS, HIGH);
    spi->endTransaction();
}
