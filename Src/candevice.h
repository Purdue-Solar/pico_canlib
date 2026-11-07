#pragma once

#include "canlib.h"

#if defined(CAN_TARGET_RP2350)

#include "rp2350/pico_canlib.h"
using CANDevice = pico_canlib;

#elif defined(CAN_TARGET_ESP32)

#include "esp32/esp_canlib.h"
using CANDevice = esp_canlib;

#elif defined(CAN_TARGET_TEENSY)

#include "teensy/teensy_canlib.h"
using CANDevice = teensy_canlib;

#else

#error "No CAN platform selected"

#endif