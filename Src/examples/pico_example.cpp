
#include <cstdint>
#include <cstdio>

#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "rp2350/pico_canlib.h"

static constexpr uint32_t CAN_ID = 0x124;
static constexpr int CAN_BAUD = 500000;

static constexpr uint8_t CAN_INT_PIN = 8;
static constexpr uint8_t CAN_MISO_PIN = 12;
static constexpr uint8_t CAN_MOSI_PIN = 11;
static constexpr uint8_t CAN_CS_PIN = 9;
static constexpr uint8_t CAN_SCK_PIN = 10;

static pico_canlib can(
    CAN_INT_PIN,
    CAN_MISO_PIN,
    CAN_MOSI_PIN,
    CAN_CS_PIN,
    CAN_SCK_PIN,
    spi1,
    CAN_BAUD);

static void processCAN()
{
    uint8_t buffer[13] = {};

    while (true)
    {
        status result = can.receiveCAN(buffer, 4, 8);

        if (result == status::NO_NEW_MESSAGE)
        {
            break;
        }

        if (result != status::SUCCESS)
        {
            printf("CAN RX error: %u\n", static_cast<unsigned>(result));
            break;
        }

        uint32_t id = static_cast<uint32_t>(buffer[0]) |
                      (static_cast<uint32_t>(buffer[1]) << 8) |
                      (static_cast<uint32_t>(buffer[2]) << 16) |
                      (static_cast<uint32_t>(buffer[3]) << 24);

        uint8_t length = buffer[4];

        printf("RX ID: 0x%03lX, Length: %u\n", static_cast<unsigned long>(id), length);

        for (uint8_t i = 0; i < length; i++)
        {
            printf("Data[%u]: 0x%02X\n", i, buffer[5 + i]);
        }
    }
}

static void transmitCAN(uint32_t count)
{
    uint8_t payload[2] = {
        0x02,
        static_cast<uint8_t>(count)};

    status result = can.transmitCAN(CAN_ID, payload, sizeof(payload));

    if (result == status::SUCCESS)
    {
        printf("TX queued: 0x%03lX\n", static_cast<unsigned long>(CAN_ID));
    }
    else
    {
        printf("CAN TX error: %u\n", static_cast<unsigned>(result));
    }
}

int main()
{
    stdio_init_all();

    // Give USB serial a chance to connect.
    sleep_ms(2000);

    printf("Initializing RP2350 CAN\n");

    status result = can.init();

    if (result != status::SUCCESS)
    {
        printf("CAN initialization failed: %u\n", static_cast<unsigned>(result));

        while (true)
        {
            sleep_ms(1000);
        }
    }

    result = can.init_irq();

    if (result != status::SUCCESS)
    {
        printf("CAN IRQ initialization failed: %u\n", static_cast<unsigned>(result));

        while (true)
        {
            sleep_ms(1000);
        }
    }

    printf("CAN initialized at %d baud\n", can.getBaud());

    absolute_time_t lastTx = get_absolute_time();
    uint32_t txCount = 0;

    while (true)
    {
        // CAN frame reads happen here, not inside the GPIO ISR.
        processCAN();

        absolute_time_t now = get_absolute_time();

        if (absolute_time_diff_us(lastTx, now) >= 1000000)
        {
            lastTx = now;
            transmitCAN(txCount++);
        }

        sleep_ms(10);
    }

    return 0;
}
