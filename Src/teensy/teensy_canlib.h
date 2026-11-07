#pragma once

#include "canlib.h"
#include <FlexCAN_T4.h>

class teensy_canlib : public canlib
{
public:
    explicit teensy_canlib(int baud = 500000);
    ~teensy_canlib() override;

    status init() override;
    status init_irq() override;
    void can_isr() override;

    status filtersAndMasks(const uint8_t *data, uint8_t length, uint32_t addr) override;

    status reset() override;

    status transmitCAN(uint32_t can_id, uint8_t *data_buffer, uint8_t data_length) override;

    status receiveCAN(uint8_t *buffer, uint8_t idSize, uint8_t bufferSize) override;

    status checkStatus(uint8_t *statusByte) override;

private:
    static constexpr uint8_t RX_QUEUE_SIZE = 16;

    FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can;

    bool initialized = false;
    bool irqInitialized = false;

    CAN_message_t rxQueue[RX_QUEUE_SIZE] = {};

    uint8_t rxHead = 0;
    uint8_t rxTail = 0;
    uint8_t rxCount = 0;

    bool rxOverflow = false;

    static teensy_canlib *activeInstance;

    static void receiveCallback(const CAN_message_t &message);

    void queueMessage(const CAN_message_t &message);
};