#pragma once

#include <cstdint>

enum class status : uint16_t
{
    SUCCESS = 0,
    TX_ID_COMMAND_ERROR = 1,
    TX_PAYLOAD_COMMAND_ERROR = 2,
    TX_ID_ERROR = 3,
    TX_PAYLOAD_ERROR = 4,
    RX_ID_ERROR = 5,
    RX_PAYLOAD_ERROR = 6,
    STATUS_ERROR = 7,
    STATUS_DONE = 8,
    STATUS_STALL = 9,
    TX_STATUS_ERROR = 10,
    TX_STATUS_DONE = 11,
    TX_STATUS_STALL = 12,
    RESET_ERROR = 13,
    RESET_INIT_ERROR = 14,
    REQUESTTS_ERROR = 15,
    MODIFIED_ERROR = 16,
    GET_CONTROL_BITS_ERROR = 17,
    GET_CONTROL_BITS_INSTR_ERROR = 18,
    SET_CONTROL_BITS_ERROR = 19,
    WRITE_ERROR = 20,
    INIT_ERROR = 21,
    RX_STATUS_ERROR = 22,
    NO_NEW_MESSAGE = 23
};

class canlib
{
public:
    canlib();
    canlib(int baud);
    virtual ~canlib();

    virtual status init() = 0;

    virtual status init_irq() = 0;

    virtual void can_isr() = 0;

    virtual status filtersAndMasks(const uint8_t *data, uint8_t length, uint32_t addr) = 0;

    virtual status reset() = 0;

    virtual status transmitCAN(uint32_t can_id, uint8_t *data_buffer, uint8_t data_lengthy) = 0;

    virtual status receiveCAN(uint8_t *buffer, uint8_t idSize, uint8_t bufferSize) = 0;

    virtual status checkStatus(uint8_t *status) = 0;

    int getBaud() const { return baud; }

private:
    int baud;
};
