#pragma once

#include <stdint.h>
#include <stddef.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "XL2515.hpp"

/// @brief RP2350 functions to communicate with XL2515 CAN tranciever
class pico_canlib
{
public:
    /// @brief Class constructor
    /// @param miso Master In Slave Out (MISO) pin
    /// @param mosi Master Out Slave In (MOSI) pin
    /// @param cs   Chip Select Pin
    /// @param sck  Serial CLK pin
    /// @param spi_hw SPI peripheral address
    pico_canlib(uint miso = 16, uint mosi =17, uint cs = 18, uint sck = 19, spi_inst_t * spi_hw = spi0) : in_miso(miso), in_mosi(mosi), in_cs(cs), in_sck(sck), in_spi_hw(spi_hw) {};
    ~pico_canlib();

    void init();
    bool sendCAN(uint8_t buffer_num, uint8_t * id, uint8_t idSize, uint8_t* data, size_t length);
    bool receiveCAN(uint8_t RX_ID, uint8_t* buffer, uint8_t idSize, uint8_t bufferSize);
    void reset();
    bool checkStatus(uint8_t RX_ID);
private:
    uint in_miso;
    uint in_mosi;
    uint in_cs;
    uint in_sck;
    spi_inst_t * in_spi_hw;
    void requestTS(uint8_t buffer);
};