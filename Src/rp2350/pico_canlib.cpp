#include "pico_canlib.h"

namespace
{
    struct SpiBusyGuard
    {
        volatile uint8_t &depth;

        explicit SpiBusyGuard(volatile uint8_t &d) : depth(d)
        {
            depth = depth + 1;
        }

        ~SpiBusyGuard()
        {
            depth = depth - 1;
        }
    };
}

pico_canlib *pico_canlib::activeInstance = nullptr;

pico_canlib::~pico_canlib()
{
    if (activeInstance == this)
    {
        gpio_set_irq_enabled(in_intGPIO, GPIO_IRQ_EDGE_FALL, false);
        activeInstance = nullptr;
    }
}

status pico_canlib::init()
{
    if (initialized)
    {
        return status::SUCCESS;
    }

    SpiBusyGuard busy(spi_depth);

    // Initialize the XL2515 SPI interface.
    spi_init(in_spi_hw, XL2515::XL2515_BAUDRATE);

    gpio_set_function(in_miso, GPIO_FUNC_SPI);
    gpio_set_function(in_mosi, GPIO_FUNC_SPI);
    gpio_set_function(in_cs, GPIO_FUNC_SIO);
    gpio_set_function(in_sck, GPIO_FUNC_SPI);

    gpio_init(in_cs);
    gpio_set_dir(in_cs, GPIO_OUT);
    gpio_put(in_cs, 1);

    gpio_init(in_intGPIO);
    gpio_set_dir(in_intGPIO, GPIO_IN);
    gpio_pull_up(in_intGPIO);

    status result = reset();

    if (result != status::SUCCESS)
    {
        return result;
    }

    // Allow the XL2515 oscillator to stabilize.
    sleep_us(50);

    uint8_t mode = 0;

    // Clear filter and mask registers.
    // All valid CAN IDs are accepted.

    static const uint8_t zeros12[12] = {0};
    static const uint8_t zeros8[8] = {0};

    if (filtersAndMasks(zeros12, sizeof(zeros12),
                        static_cast<uint32_t>(XL2515::IN_ADDR::RXF0SIDH)) != status::SUCCESS)
    {
        return status::INIT_ERROR;
    }

    if (filtersAndMasks(zeros12, sizeof(zeros12),
                        static_cast<uint32_t>(XL2515::IN_ADDR::RXF3SIDH)) != status::SUCCESS)
    {
        return status::INIT_ERROR;
    }

    if (filtersAndMasks(zeros8, sizeof(zeros8),
                        static_cast<uint32_t>(XL2515::IN_ADDR::RXM0SIDH)) != status::SUCCESS)
    {
        return status::INIT_ERROR;
    }

    // Configure receive buffer 0 with rollover enabled.

    if (setByte(XL2515::RXB0CTRL_RXALL_BUKT,
                XL2515::IN_ADDR::RXB0CTRL) != status::SUCCESS)
    {
        return status::INIT_ERROR;
    }

    if (getByte(&mode, XL2515::IN_ADDR::RXB0CTRL) != status::SUCCESS)
    {
        return status::INIT_ERROR;
    }

    if ((mode & 0x6C) != XL2515::RXB0CTRL_RXALL_BUKT)
    {
        return status::INIT_ERROR;
    }

    // Configure receive buffer 1.

    if (setByte(XL2515::RXB1CTRL_RXALL,
                XL2515::IN_ADDR::RXB1CTRL) != status::SUCCESS)
    {
        return status::INIT_ERROR;
    }

    if (getByte(&mode, XL2515::IN_ADDR::RXB1CTRL) != status::SUCCESS)
    {
        return status::INIT_ERROR;
    }

    if ((mode & 0x60) != XL2515::RXB1CTRL_RXALL)
    {
        return status::INIT_ERROR;
    }

    // CAN bit timing.
    // These constants must be configured for the board's 16 MHz oscillator.

    if (setByte(XL2515::NORMAL_CNF1,
                XL2515::IN_ADDR::CNF1) != status::SUCCESS)
    {
        return status::INIT_ERROR;
    }

    if (getByte(&mode, XL2515::IN_ADDR::CNF1) != status::SUCCESS)
    {
        return status::INIT_ERROR;
    }

    if (mode != XL2515::NORMAL_CNF1)
    {
        return status::INIT_ERROR;
    }

    if (setByte(XL2515::NORMAL_CNF2,
                XL2515::IN_ADDR::CNF2) != status::SUCCESS)
    {
        return status::INIT_ERROR;
    }

    if (getByte(&mode, XL2515::IN_ADDR::CNF2) != status::SUCCESS)
    {
        return status::INIT_ERROR;
    }

    if (mode != XL2515::NORMAL_CNF2)
    {
        return status::INIT_ERROR;
    }

    if (setByte(XL2515::NORMAL_CNF3,
                XL2515::IN_ADDR::CNF3) != status::SUCCESS)
    {
        return status::INIT_ERROR;
    }

    if (getByte(&mode, XL2515::IN_ADDR::CNF3) != status::SUCCESS)
    {
        return status::INIT_ERROR;
    }

    if (mode != XL2515::NORMAL_CNF3)
    {
        return status::INIT_ERROR;
    }

    // Leave interrupts disabled until init_irq() is called.

    if (setByte(0x00, XL2515::IN_ADDR::CANINTE) != status::SUCCESS)
    {
        return status::INIT_ERROR;
    }

    // Enter Normal mode.

    if (setByte(XL2515::NORMAL_MODE,
                XL2515::IN_ADDR::CANCTRL) != status::SUCCESS)
    {
        return status::INIT_ERROR;
    }

    mode = 0xFF;

    for (int attempt = 0; attempt < 10; attempt++)
    {
        if (getByte(&mode, XL2515::IN_ADDR::CANSTAT) != status::SUCCESS)
        {
            mode = 0xFF;
            sleep_us(100);
            continue;
        }

        if ((mode & 0xE0) == 0x00)
        {
            break;
        }

        sleep_us(100);
    }

    if ((mode & 0xE0) != 0x00)
    {
        return status::INIT_ERROR;
    }

    initialized = true;

    return status::SUCCESS;
}

status pico_canlib::init_irq()
{
    if (!initialized)
    {
        return status::INIT_ERROR;
    }

    if (irqInitialized)
    {
        return status::SUCCESS;
    }

    if (activeInstance != nullptr && activeInstance != this)
    {
        return status::INIT_ERROR;
    }

    // Enable RX0IE and RX1IE on the XL2515.
    // CANINTE bits 0 and 1 correspond to RX0 and RX1 interrupts.

    if (setByte(0x03, XL2515::IN_ADDR::CANINTE) != status::SUCCESS)
    {
        return status::INIT_ERROR;
    }

    activeInstance = this;

    irqPending = gpio_get(in_intGPIO) == 0;

    // XL2515 INT is active-low.
    // Only mark pending in the interrupt handler; do not use SPI there.

    gpio_set_irq_enabled_with_callback(
        in_intGPIO,
        GPIO_IRQ_EDGE_FALL,
        true,
        gpioIRQCallback);

    irqInitialized = true;

    return status::SUCCESS;
}

void pico_canlib::gpioIRQCallback(uint gpio, uint32_t events)
{
    if (activeInstance == nullptr)
    {
        return;
    }

    if (gpio != activeInstance->in_intGPIO)
    {
        return;
    }

    if (events & GPIO_IRQ_EDGE_FALL)
    {
        activeInstance->can_isr();
    }
}

void pico_canlib::can_isr()
{
    // Interrupt context: never perform SPI operations here.
    irqPending = true;
}

status pico_canlib::filtersAndMasks(const uint8_t *data, uint8_t length, uint32_t addr)
{
    SpiBusyGuard busy(spi_depth);

    if (addr > 0xFF || (length > 0 && data == nullptr))
    {
        return status::WRITE_ERROR;
    }

    uint8_t header[2] = {
        static_cast<uint8_t>(XL2515::SPI_INSTR_XL::WRITE),
        static_cast<uint8_t>(addr)};

    gpio_put(in_cs, 0);

    bool ok = spi_write_blocking(in_spi_hw, header, 2) == 2;

    if (ok && length > 0)
    {
        ok = spi_write_blocking(in_spi_hw, data, length) == length;
    }

    gpio_put(in_cs, 1);

    return ok ? status::SUCCESS : status::WRITE_ERROR;
}

status pico_canlib::setByte(uint8_t bytes, XL2515::IN_ADDR addr)
{
    SpiBusyGuard busy(spi_depth);

    uint8_t message[3] = {
        static_cast<uint8_t>(XL2515::SPI_INSTR_XL::WRITE),
        static_cast<uint8_t>(addr),
        bytes};

    gpio_put(in_cs, 0);

    int written = spi_write_blocking(in_spi_hw, message, 3);

    gpio_put(in_cs, 1);

    return written == 3 ? status::SUCCESS : status::WRITE_ERROR;
}

status pico_canlib::getByte(uint8_t *bytes, XL2515::IN_ADDR addr)
{
    SpiBusyGuard busy(spi_depth);

    if (bytes == nullptr)
    {
        return status::GET_CONTROL_BITS_ERROR;
    }

    uint8_t data[2] = {
        static_cast<uint8_t>(XL2515::SPI_INSTR_XL::READ),
        static_cast<uint8_t>(addr)};

    gpio_put(in_cs, 0);

    if (spi_write_blocking(in_spi_hw, data, 2) != 2)
    {
        gpio_put(in_cs, 1);
        return status::GET_CONTROL_BITS_INSTR_ERROR;
    }

    if (spi_read_blocking(in_spi_hw, 0, bytes, 1) != 1)
    {
        gpio_put(in_cs, 1);
        return status::GET_CONTROL_BITS_ERROR;
    }

    gpio_put(in_cs, 1);

    return status::SUCCESS;
}

status pico_canlib::reset()
{
    SpiBusyGuard busy(spi_depth);

    if (irqInitialized)
    {
        gpio_set_irq_enabled(in_intGPIO, GPIO_IRQ_EDGE_FALL, false);
        irqInitialized = false;
    }

    if (activeInstance == this)
    {
        activeInstance = nullptr;
    }

    initialized = false;
    irqPending = false;

    uint8_t instr = static_cast<uint8_t>(XL2515::SPI_INSTR_XL::RESET);

    gpio_put(in_cs, 0);

    int written = spi_write_blocking(in_spi_hw, &instr, 1);

    gpio_put(in_cs, 1);

    return written == 1 ? status::SUCCESS : status::RESET_ERROR;
}

status pico_canlib::requestTS(uint8_t buffer)
{
    SpiBusyGuard busy(spi_depth);

    if (buffer > 2)
    {
        return status::REQUESTTS_ERROR;
    }

    uint8_t instr = static_cast<uint8_t>(XL2515::SPI_INSTR_XL::REQTS) |
                    static_cast<uint8_t>(1U << buffer);

    gpio_put(in_cs, 0);

    int written = spi_write_blocking(in_spi_hw, &instr, 1);

    gpio_put(in_cs, 1);

    return written == 1 ? status::SUCCESS : status::REQUESTTS_ERROR;
}

status pico_canlib::checkStatus(uint8_t *statusByte)
{
    SpiBusyGuard busy(spi_depth);

    if (statusByte == nullptr)
    {
        return status::STATUS_ERROR;
    }

    uint8_t instr = static_cast<uint8_t>(XL2515::SPI_INSTR_XL::READ_STATUS);

    gpio_put(in_cs, 0);

    if (spi_write_blocking(in_spi_hw, &instr, 1) != 1)
    {
        gpio_put(in_cs, 1);
        return status::STATUS_ERROR;
    }

    if (spi_read_blocking(in_spi_hw, 0, statusByte, 1) != 1)
    {
        gpio_put(in_cs, 1);
        return status::STATUS_ERROR;
    }

    gpio_put(in_cs, 1);

    return status::SUCCESS;
}

status pico_canlib::modifiedBit(uint8_t bytes, uint8_t address, uint8_t masked)
{
    SpiBusyGuard busy(spi_depth);

    XL2515::bit_modify msg;

    msg.addr = address;
    msg.mask = masked;
    msg.payload = bytes;

    gpio_put(in_cs, 0);

    int written = spi_write_blocking(
        in_spi_hw,
        reinterpret_cast<uint8_t *>(&msg),
        4);

    gpio_put(in_cs, 1);

    return written == 4 ? status::SUCCESS : status::MODIFIED_ERROR;
}

status pico_canlib::transmitCAN(uint32_t can_id, uint8_t *data_buffer, uint8_t data_length)
{
    SpiBusyGuard busy(spi_depth);

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

    // Use TXB0.
    // TXREQ bit 3 indicates that the buffer is occupied.

    uint8_t ctrl = 0;

    if (getByte(&ctrl, XL2515::IN_ADDR::TXBxCTRL) != status::SUCCESS)
    {
        return status::TX_STATUS_ERROR;
    }

    if (ctrl & 0x08)
    {
        return status::TX_STATUS_STALL;
    }

    // Standard 11-bit ID encoded as SIDH and SIDL.
    // EXIDE is cleared.

    uint8_t id[4] = {
        static_cast<uint8_t>(can_id >> 3),
        static_cast<uint8_t>((can_id & 0x07) << 5),
        0,
        0};

    if (filtersAndMasks(id, 4,
                        static_cast<uint32_t>(XL2515::IN_ADDR::TXBxSIDH)) != status::SUCCESS)
    {
        return status::TX_ID_COMMAND_ERROR;
    }

    // Configure DLC, RTR = 0.

    if (setByte(data_length, XL2515::IN_ADDR::TXBxDLC) != status::SUCCESS)
    {
        return status::TX_PAYLOAD_COMMAND_ERROR;
    }

    // Load TXB0 payload, starting at data register 0.

    uint8_t instr = 0x41;

    gpio_put(in_cs, 0);

    bool ok = spi_write_blocking(in_spi_hw, &instr, 1) == 1;

    if (ok && data_length > 0)
    {
        ok = spi_write_blocking(in_spi_hw, data_buffer, data_length) == data_length;
    }

    gpio_put(in_cs, 1);

    if (!ok)
    {
        return status::TX_PAYLOAD_COMMAND_ERROR;
    }

    return requestTS(0);
}

status pico_canlib::receiveCAN(uint8_t *buffer, uint8_t idSize, uint8_t bufferSize)
{
    SpiBusyGuard busy(spi_depth);

    if (!initialized || buffer == nullptr)
    {
        return status::RX_PAYLOAD_ERROR;
    }

    // The interrupt is advisory. Always checking the controller flags
    // prevents missed interrupts from permanently blocking reception.

    if (irqInitialized && !irqPending && gpio_get(in_intGPIO) != 0)
    {
        return status::NO_NEW_MESSAGE;
    }

    uint8_t st = 0;

    status result = checkStatus(&st);

    if (result != status::SUCCESS)
    {
        return result;
    }

    uint8_t rx = st & 0x03;

    if (rx == 0)
    {
        irqPending = gpio_get(in_intGPIO) == 0;
        return status::NO_NEW_MESSAGE;
    }

    // Read RXB0 first if both buffers contain a frame.

    uint8_t RX_ID = (rx & 0x01) ? 0 : 1;

    uint8_t instr = static_cast<uint8_t>(XL2515::SPI_INSTR_XL::READ_RX_BUFF) |
                    static_cast<uint8_t>(RX_ID << 2);

    gpio_put(in_cs, 0);

    if (spi_write_blocking(in_spi_hw, &instr, 1) != 1)
    {
        gpio_put(in_cs, 1);
        return status::RX_ID_ERROR;
    }

    // Read SIDH, SIDL, EID8, EID0, DLC and 8 payload bytes.

    if (spi_read_blocking(in_spi_hw, 0, buffer, 13) != 13)
    {
        gpio_put(in_cs, 1);
        return status::RX_PAYLOAD_ERROR;
    }

    // Deasserting CS completes the RX buffer read and clears RXnIF.

    gpio_put(in_cs, 1);

    irqPending = gpio_get(in_intGPIO) == 0;

    // Reject extended and remote frames.

    if ((buffer[1] & 0x08) || (buffer[4] & 0x40))
    {
        return status::RX_ID_ERROR;
    }

    // Decode standard CAN identifier.

    uint32_t id = (static_cast<uint32_t>(buffer[0]) << 3) |
                  (static_cast<uint32_t>(buffer[1]) >> 5);

    buffer[0] = static_cast<uint8_t>(id);
    buffer[1] = static_cast<uint8_t>(id >> 8);
    buffer[2] = static_cast<uint8_t>(id >> 16);
    buffer[3] = static_cast<uint8_t>(id >> 24);

    buffer[4] &= 0x0F;

    if (buffer[4] > 8)
    {
        return status::RX_PAYLOAD_ERROR;
    }

    return status::SUCCESS;
}