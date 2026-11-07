#pragma once

#include <cstdint>

#include "canlib.h"
#include "MCP2515.h"

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

class pico_canlib : public canlib
{
public:
    pico_canlib(uint8_t gpioInt = 8, uint8_t miso = 12, uint8_t mosi = 11,
                uint8_t cs = 9, uint8_t sck = 10, spi_inst_t *spi_hw = spi1,
                int baud = 500000)
        : canlib(baud),
          in_intGPIO(gpioInt),
          in_miso(miso),
          in_mosi(mosi),
          in_cs(cs),
          in_sck(sck),
          in_spi_hw(spi_hw)
    {
    }

    ~pico_canlib() override;

    status init() override;
    status init_irq() override;
    void can_isr() override;

    status filtersAndMasks(const uint8_t *data, uint8_t length, uint32_t addr) override;

    status reset() override;

    status transmitCAN(uint32_t can_id, uint8_t *data_buffer, uint8_t data_length) override;

    status receiveCAN(uint8_t *buffer, uint8_t idSize, uint8_t bufferSize) override;

    status checkStatus(uint8_t *statusByte) override;

    status getByte(uint8_t *bytes, XL2515::IN_ADDR addr);
    status setByte(uint8_t bytes, XL2515::IN_ADDR addr);

    volatile uint8_t spi_depth = 0;

private:
    uint8_t in_intGPIO;
    uint8_t in_miso;
    uint8_t in_mosi;
    uint8_t in_cs;
    uint8_t in_sck;

    spi_inst_t *in_spi_hw;

    bool initialized = false;
    bool irqInitialized = false;
    volatile bool irqPending = false;

    static pico_canlib *activeInstance;

    static void gpioIRQCallback(uint gpio, uint32_t events);

    status requestTS(uint8_t buffer);
    status modifiedBit(uint8_t bytes, uint8_t address, uint8_t masked);
};