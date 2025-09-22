#pragma once

#include <stdint.h>
#include <stddef.h>

class XL2515_Driver
{
public:
    XL2515_Driver();
    ~XL2515_Driver();

    void init(void);

    void sendData(const uint8_t* data, size_t length);
    size_t receiveData(uint8_t* buffer, size_t bufferSize);
};