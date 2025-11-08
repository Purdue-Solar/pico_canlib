#pragma once
#include "pico/stdlib.h"

class artemis_canid{
    public:
        static constexpr uint32_t tempSOC = 0x200;
        static constexpr uint32_t battDiagnostic = 0x202;
        static constexpr uint32_t setMotorCurrent = 0x402;
};
