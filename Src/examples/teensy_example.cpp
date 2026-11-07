#include <Arduino.h>

#include "teensy/teensy_canlib.h"

static constexpr uint32_t CAN_ID = 0x125;
static constexpr int CAN_BAUD = 500000;

static teensy_canlib can(CAN_BAUD);

static bool canReady = false;
static uint32_t lastTxMs = 0;
static uint32_t txCount = 0;

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
            Serial.printf("CAN RX error: %u\n", static_cast<unsigned>(result));
            break;
        }

        uint32_t id = static_cast<uint32_t>(buffer[0]) |
                      (static_cast<uint32_t>(buffer[1]) << 8) |
                      (static_cast<uint32_t>(buffer[2]) << 16) |
                      (static_cast<uint32_t>(buffer[3]) << 24);

        uint8_t length = buffer[4];

        Serial.printf("RX ID: 0x%03lX, Length: %u\n", static_cast<unsigned long>(id), length);

        for (uint8_t i = 0; i < length; i++)
        {
            Serial.printf("Data[%u]: 0x%02X\n", i, buffer[5 + i]);
        }
    }
}

static void transmitCAN(uint32_t count)
{
    uint8_t payload[2] = {
        0x03,
        static_cast<uint8_t>(count)};

    status result = can.transmitCAN(CAN_ID, payload, sizeof(payload));

    if (result == status::SUCCESS)
    {
        Serial.printf("TX queued: 0x%03lX\n", static_cast<unsigned long>(CAN_ID));
    }
    else
    {
        Serial.printf("CAN TX error: %u\n", static_cast<unsigned>(result));
    }
}

void setup()
{
    Serial.begin(115200);

    uint32_t startMs = millis();

    while (!Serial && millis() - startMs < 2000)
    {
    }

    Serial.println("Initializing Teensy CAN");

    status result = can.init();

    if (result != status::SUCCESS)
    {
        Serial.printf("CAN initialization failed: %u\n", static_cast<unsigned>(result));
        return;
    }

    result = can.init_irq();

    if (result != status::SUCCESS)
    {
        Serial.printf("CAN IRQ initialization failed: %u\n", static_cast<unsigned>(result));
        return;
    }

    canReady = true;
    lastTxMs = millis();

    Serial.printf("CAN initialized at %d baud\n", can.getBaud());
}

void loop()
{
    if (!canReady)
    {
        delay(1000);
        return;
    }

    // receiveCAN() dispatches pending FlexCAN_T4 events.
    processCAN();

    uint32_t now = millis();

    if (now - lastTxMs >= 1000)
    {
        lastTxMs = now;
        transmitCAN(txCount++);
    }

    delay(10);
}
