#pragma once

#include <stdint.h>
#include <stddef.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

class pico_canlib
{
public:
    pico_canlib();
    ~pico_canlib();

    void init(void);
    bool sendData(const uint8_t* data, size_t length);
    bool receiveData(uint8_t* buffer, size_t bufferSize);
private:

};