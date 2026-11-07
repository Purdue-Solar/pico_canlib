#include "teensy_canlib.h"

#include <cstring>

teensy_canlib *teensy_canlib::activeInstance = nullptr;

teensy_canlib::teensy_canlib(int baud) : canlib(baud)
{
}

teensy_canlib::~teensy_canlib()
{
    if (activeInstance == this)
    {
        activeInstance = nullptr;
    }
}

status teensy_canlib::init()
{
    if (initialized)
    {
        return status::SUCCESS;
    }

    if (getBaud() <= 0 || getBaud() > 1000000)
    {
        return status::INIT_ERROR;
    }

    can.begin();
    can.setBaudRate(getBaud());
    can.setMBFilter(ACCEPT_ALL);

    rxHead = 0;
    rxTail = 0;
    rxCount = 0;
    rxOverflow = false;

    initialized = true;

    return status::SUCCESS;
}

status teensy_canlib::init_irq()
{
    if (!initialized)
    {
        return status::INIT_ERROR;
    }

    if (irqInitialized)
    {
        return status::SUCCESS;
    }

    // This implementation uses one CAN1 controller instance.
    if (activeInstance != nullptr && activeInstance != this)
    {
        return status::INIT_ERROR;
    }

    activeInstance = this;

    // Register callback for all receive mailboxes.
    can.onReceive(receiveCallback);

    // Enable mailbox receive interrupts.
    can.enableMBInterrupts();

    irqInitialized = true;

    return status::SUCCESS;
}

void teensy_canlib::can_isr()
{
    if (!initialized || !irqInitialized)
    {
        return;
    }

    // FlexCAN_T4 stores received interrupt frames internally.
    // events() dispatches them to receiveCallback().
    //
    // This function must run outside the hardware ISR.
    can.events();
}

void teensy_canlib::receiveCallback(const CAN_message_t &message)
{
    if (activeInstance != nullptr)
    {
        activeInstance->queueMessage(message);
    }
}

void teensy_canlib::queueMessage(const CAN_message_t &message)
{
    if (message.flags.extended || message.flags.remote || message.len > 8)
    {
        return;
    }

    if (rxCount >= RX_QUEUE_SIZE)
    {
        rxOverflow = true;
        return;
    }

    rxQueue[rxHead] = message;

    rxHead = (rxHead + 1) % RX_QUEUE_SIZE;
    rxCount++;
}

status teensy_canlib::filtersAndMasks(const uint8_t *data, uint8_t length, uint32_t addr)
{
    // MCP2515 register writes do not apply to FlexCAN.

    (void)data;
    (void)length;
    (void)addr;

    return status::SET_CONTROL_BITS_ERROR;
}

status teensy_canlib::reset()
{
    if (!initialized)
    {
        return status::RESET_INIT_ERROR;
    }

    bool restoreIRQ = irqInitialized;

    initialized = false;
    irqInitialized = false;

    if (activeInstance == this)
    {
        activeInstance = nullptr;
    }

    if (init() != status::SUCCESS)
    {
        return status::RESET_ERROR;
    }

    if (restoreIRQ && init_irq() != status::SUCCESS)
    {
        return status::RESET_ERROR;
    }

    return status::SUCCESS;
}

status teensy_canlib::transmitCAN(uint32_t can_id, uint8_t *data_buffer, uint8_t data_length)
{
    if (!initialized)
    {
        return status::TX_STATUS_ERROR;
    }

    if (can_id > 0x7FF)
    {
        return status::TX_ID_ERROR;
    }

    if (data_length > 8 || (data_length > 0 && data_buffer == nullptr))
    {
        return status::TX_PAYLOAD_ERROR;
    }

    CAN_message_t message = {};

    message.id = can_id;
    message.flags.extended = false;
    message.flags.remote = false;
    message.len = data_length;

    if (data_length > 0)
    {
        std::memcpy(message.buf, data_buffer, data_length);
    }

    if (can.write(message))
    {
        return status::SUCCESS;
    }

    return status::TX_STATUS_ERROR;
}

status teensy_canlib::receiveCAN(uint8_t *buffer, uint8_t idSize, uint8_t bufferSize)
{
    if (!initialized || buffer == nullptr || idSize != 4 || bufferSize != 8)
    {
        return status::RX_PAYLOAD_ERROR;
    }

    CAN_message_t message = {};

    if (irqInitialized)
    {
        // Dispatch queued FlexCAN interrupt events.
        can_isr();

        if (rxCount == 0)
        {
            if (rxOverflow)
            {
                rxOverflow = false;
                return status::RX_STATUS_ERROR;
            }

            return status::NO_NEW_MESSAGE;
        }

        message = rxQueue[rxTail];

        rxTail = (rxTail + 1) % RX_QUEUE_SIZE;
        rxCount--;
    }
    else
    {
        // Polling mode when init_irq() has not been called.
        if (!can.read(message))
        {
            return status::NO_NEW_MESSAGE;
        }
    }

    if (message.flags.extended || message.flags.remote || message.id > 0x7FF)
    {
        return status::RX_ID_ERROR;
    }

    if (message.len > 8)
    {
        return status::RX_PAYLOAD_ERROR;
    }

    std::memset(buffer, 0, 13);

    uint32_t id = message.id;

    buffer[0] = static_cast<uint8_t>(id);
    buffer[1] = static_cast<uint8_t>(id >> 8);
    buffer[2] = static_cast<uint8_t>(id >> 16);
    buffer[3] = static_cast<uint8_t>(id >> 24);
    buffer[4] = message.len;

    if (message.len > 0)
    {
        std::memcpy(buffer + 5, message.buf, message.len);
    }

    return status::SUCCESS;
}

status teensy_canlib::checkStatus(uint8_t *statusByte)
{
    if (!initialized || statusByte == nullptr)
    {
        return status::STATUS_ERROR;
    }

    // Software status:
    // Bit 0: Initialized
    // Bit 1: Interrupt handling enabled
    // Bit 4: Received frames pending
    // Bit 6: Software receive queue overflow

    *statusByte = 0x01;

    if (irqInitialized)
    {
        *statusByte |= 0x02;

        // Dispatch any pending receive callbacks.
        can_isr();
    }

    if (rxCount > 0)
    {
        *statusByte |= 0x10;
    }

    if (rxOverflow)
    {
        *statusByte |= 0x40;
    }

    return status::SUCCESS;
}