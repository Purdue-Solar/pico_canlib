#pragma once

#include "canlib.h"

#include "driver/twai.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class esp_canlib : public canlib
{
public:
    explicit esp_canlib(int baud = 500000, int txPin = 27, int rxPin = 33);
    ~esp_canlib() override;

    status init() override;
    status init_irq() override;
    void can_isr() override;

    status filtersAndMasks(const uint8_t *data, uint8_t length, uint32_t addr) override;

    status reset() override;

    status transmitCAN(uint32_t can_id, uint8_t *data_buffer, uint8_t data_length) override;

    status receiveCAN(uint8_t *buffer, uint8_t idSize, uint8_t bufferSize) override;

    status checkStatus(uint8_t *statusByte) override;

private:
    int txPin;
    int rxPin;

    bool initialized = false;
    bool irqInitialized = false;

    volatile bool irqTaskRunning = false;
    volatile bool rxPending = false;

    volatile TaskHandle_t irqTaskHandle = nullptr;

    bool getTiming(twai_timing_config_t &config);

    static void irqTaskEntry(void *arg);
};