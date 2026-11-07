
#include <Arduino.h>
#include <cstdint>

#include "artemis_canid.hpp"
#include "teensy/teensy_canlib.h"

static constexpr int CAN_BAUD = 500000;

static constexpr MessageID TEST_MESSAGE = MessageID::MotorSpeed;
static constexpr uint32_t CAN_ID = getID(TEST_MESSAGE);
static constexpr uint8_t CAN_LENGTH = getNumCanBytesMessage(TEST_MESSAGE);
static constexpr uint32_t CAN_PERIOD_MS = getMessageDefinition(TEST_MESSAGE).periodMs;

static teensy_canlib teensycan1(CAN_BAUD);
static canlib &can1 = teensycan1;

static bool canReady = false;
static uint32_t lastTx = 0;
static uint32_t counter = 0;

static void transmitTest(uint32_t count)
{
    uint8_t data[8] = {};

    data[0] = static_cast<uint8_t>(count);
    data[1] = static_cast<uint8_t>(count >> 8);
    data[2] = static_cast<uint8_t>(count >> 16);
    data[3] = static_cast<uint8_t>(count >> 24);

    status result = can1.transmitCAN(CAN_ID, data, CAN_LENGTH);

    if (result == status::SUCCESS)
    {
        Serial.printf("TX: %s, ID=0x%03lX, Counter=%lu\n",
                      getMessageDefinition(TEST_MESSAGE).name,
                      static_cast<unsigned long>(CAN_ID),
                      static_cast<unsigned long>(count));
    }
    else
    {
        Serial.printf("TX failed: %u\n", static_cast<unsigned>(result));
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
            Serial.printf("RX failed: %u\n", static_cast<unsigned>(result));
            break;
        }

        uint32_t id = static_cast<uint32_t>(buffer[0]) |
                      (static_cast<uint32_t>(buffer[1]) << 8) |
                      (static_cast<uint32_t>(buffer[2]) << 16) |
                      (static_cast<uint32_t>(buffer[3]) << 24);

        uint8_t length = buffer[4];

        if (length > 8)
        {
            Serial.printf("Invalid CAN length: %u\n", length);
            continue;
        }

        Serial.printf("RX ID=0x%03lX, DLC=%u\n",
                      static_cast<unsigned long>(id), length);

        if (id == CAN_ID && length == CAN_LENGTH)
        {
            Serial.printf("Received: %s\n", getMessageDefinition(TEST_MESSAGE).name);

            for (uint8_t i = 0; i < length; i++)
            {
                Serial.printf("Data[%u]=0x%02X\n", i, buffer[5 + i]);
            }
        }
    }
}

void setup()
{
    Serial.begin(115200);

    uint32_t start = millis();

    while (!Serial && millis() - start < 2000)
    {
    }

    Serial.println("Starting Teensy CAN test");

    status result = can1.init();

    if (result != status::SUCCESS)
    {
        Serial.printf("CAN initialization failed: %u\n", static_cast<unsigned>(result));
        return;
    }

    result = can1.init_irq();

    if (result != status::SUCCESS)
    {
        Serial.printf("CAN IRQ initialization failed: %u\n", static_cast<unsigned>(result));
        return;
    }

    canReady = true;
    lastTx = millis();

    Serial.printf("CAN initialized at %d baud\n", can1.getBaud());

    const SignalDefinition &motorVelocity = getSignal(McSpeedSignal::MotorVelocity);
    const SignalDefinition &vehicleVelocity = getSignal(McSpeedSignal::VehicleVelocity);

    Serial.printf("%s: startBit=%u, length=%u\n",
                  motorVelocity.name, motorVelocity.startBit, motorVelocity.length);

    Serial.printf("%s: startBit=%u, length=%u\n",
                  vehicleVelocity.name, vehicleVelocity.startBit, vehicleVelocity.length);
}

void loop()
{
    if (!canReady)
    {
        delay(1000);
        return;
    }

    receiveTest();

    uint32_t now = millis();

    if (now - lastTx >= CAN_PERIOD_MS)
    {
        lastTx = now;
        transmitTest(counter++);
    }

    delay(10);
}
