
#include <cstdint>
#include <cstdio>

#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "artemis_canid.hpp"
#include "rp2350/pico_canlib.h"

static constexpr int CAN_BAUD = 500000;

static constexpr MessageID TEST_MESSAGE = MessageID::MotorSpeed;
static constexpr uint32_t CAN_ID = getID(TEST_MESSAGE);
static constexpr uint8_t CAN_LENGTH = getNumCanBytesMessage(TEST_MESSAGE);
static constexpr uint32_t CAN_PERIOD_MS = getMessageDefinition(TEST_MESSAGE).periodMs;

static pico_canlib picocan1(8, 12, 11, 9, 10, spi1, CAN_BAUD);
static canlib &can1 = picocan1;

static void transmitTest(uint32_t counter)
{
    uint8_t data[8] = {};

    data[0] = static_cast<uint8_t>(counter);
    data[1] = static_cast<uint8_t>(counter >> 8);
    data[2] = static_cast<uint8_t>(counter >> 16);
    data[3] = static_cast<uint8_t>(counter >> 24);

    status result = can1.transmitCAN(CAN_ID, data, CAN_LENGTH);

    if (result == status::SUCCESS)
    {
        printf("TX: %s, ID=0x%03lX, Counter=%lu\n",
               getMessageDefinition(TEST_MESSAGE).name,
               static_cast<unsigned long>(CAN_ID),
               static_cast<unsigned long>(counter));
    }
    else
    {
        printf("TX failed: %u\n", static_cast<unsigned>(result));
    }
}

static void receiveTest()
{
    uint8_t buffer[13] = {};

    while (true)
    {
        status result = can1.receiveCAN(buffer, 4, 8);

        if (result == status::NO_NEW_MESSAGE)
        {
            break;
        }

        if (result != status::SUCCESS)
        {
            printf("RX failed: %u\n", static_cast<unsigned>(result));
            break;
        }

        uint32_t id = static_cast<uint32_t>(buffer[0]) |
                      (static_cast<uint32_t>(buffer[1]) << 8) |
                      (static_cast<uint32_t>(buffer[2]) << 16) |
                      (static_cast<uint32_t>(buffer[3]) << 24);

        uint8_t length = buffer[4];

        if (length > 8)
        {
            printf("Invalid CAN length: %u\n", length);
            continue;
        }

        printf("RX ID=0x%03lX, DLC=%u\n",
               static_cast<unsigned long>(id), length);

        if (id == CAN_ID && length == CAN_LENGTH)
        {
            printf("Received: %s\n", getMessageDefinition(TEST_MESSAGE).name);

            for (uint8_t i = 0; i < length; i++)
            {
                printf("Data[%u]=0x%02X\n", i, buffer[5 + i]);
            }
        }
    }
}

int main()
{
    stdio_init_all();
    sleep_ms(2000);

    printf("Starting RP2350 CAN test\n");

    status result = can1.init();

    if (result != status::SUCCESS)
    {
        printf("CAN initialization failed: %u\n", static_cast<unsigned>(result));

        while (true)
        {
            sleep_ms(1000);
        }
    }

    result = can1.init_irq();

    if (result != status::SUCCESS)
    {
        printf("CAN IRQ initialization failed: %u\n", static_cast<unsigned>(result));

        while (true)
        {
            sleep_ms(1000);
        }
    }

    printf("CAN initialized at %d baud\n", can1.getBaud());

    const SignalDefinition &motorVelocity = getSignal(McSpeedSignal::MotorVelocity);
    const SignalDefinition &vehicleVelocity = getSignal(McSpeedSignal::VehicleVelocity);

    printf("%s: startBit=%u, length=%u\n",
           motorVelocity.name, motorVelocity.startBit, motorVelocity.length);

    printf("%s: startBit=%u, length=%u\n",
           vehicleVelocity.name, vehicleVelocity.startBit, vehicleVelocity.length);

    absolute_time_t lastTx = get_absolute_time();
    uint32_t counter = 0;

    while (true)
    {
        receiveTest();

        absolute_time_t now = get_absolute_time();

        if (absolute_time_diff_us(lastTx, now) >= static_cast<int64_t>(CAN_PERIOD_MS) * 1000)
        {
            lastTx = now;
            transmitTest(counter++);
        }

        sleep_ms(10);
    }
}
