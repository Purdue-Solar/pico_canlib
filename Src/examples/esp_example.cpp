
#include <cstdint>
#include <cstdio>

#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "artemis_canid.hpp"
#include "esp32/esp_canlib.h"

static constexpr char TAG[] = "CAN_TEST";
static constexpr int CAN_BAUD = 500000;

static constexpr MessageID TEST_MESSAGE = MessageID::MotorSpeed;
static constexpr uint32_t CAN_ID = getID(TEST_MESSAGE);
static constexpr uint8_t CAN_LENGTH = getNumCanBytesMessage(TEST_MESSAGE);
static constexpr uint32_t CAN_PERIOD_MS = getMessageDefinition(TEST_MESSAGE).periodMs;

static esp_canlib espcan1(CAN_BAUD, 27, 33);
static canlib &can1 = espcan1;

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
        ESP_LOGI(TAG, "TX: %s, ID=0x%03lX, Counter=%lu",
                 getMessageDefinition(TEST_MESSAGE).name,
                 static_cast<unsigned long>(CAN_ID),
                 static_cast<unsigned long>(counter));
    }
    else
    {
        ESP_LOGE(TAG, "TX failed: %u", static_cast<unsigned>(result));
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
            ESP_LOGE(TAG, "RX failed: %u", static_cast<unsigned>(result));
            break;
        }

        uint32_t id = static_cast<uint32_t>(buffer[0]) |
                      (static_cast<uint32_t>(buffer[1]) << 8) |
                      (static_cast<uint32_t>(buffer[2]) << 16) |
                      (static_cast<uint32_t>(buffer[3]) << 24);

        uint8_t length = buffer[4];

        if (length > 8)
        {
            ESP_LOGE(TAG, "Invalid CAN length: %u", length);
            continue;
        }

        ESP_LOGI(TAG, "RX ID=0x%03lX, DLC=%u",
                 static_cast<unsigned long>(id), length);

        if (id == CAN_ID && length == CAN_LENGTH)
        {
            ESP_LOGI(TAG, "Received: %s", getMessageDefinition(TEST_MESSAGE).name);

            for (uint8_t i = 0; i < length; i++)
            {
                ESP_LOGI(TAG, "Data[%u]=0x%02X", i, buffer[5 + i]);
            }
        }
    }
}

extern "C" void app_main()
{
    ESP_LOGI(TAG, "Starting ESP32 CAN test");

    status result = can1.init();

    if (result != status::SUCCESS)
    {
        ESP_LOGE(TAG, "CAN initialization failed: %u", static_cast<unsigned>(result));
        return;
    }

    result = can1.init_irq();

    if (result != status::SUCCESS)
    {
        ESP_LOGE(TAG, "CAN IRQ initialization failed: %u", static_cast<unsigned>(result));
        return;
    }

    ESP_LOGI(TAG, "CAN initialized at %d baud", can1.getBaud());

    const SignalDefinition &motorVelocity = getSignal(McSpeedSignal::MotorVelocity);
    const SignalDefinition &vehicleVelocity = getSignal(McSpeedSignal::VehicleVelocity);

    ESP_LOGI(TAG, "%s: startBit=%u, length=%u",
             motorVelocity.name, motorVelocity.startBit, motorVelocity.length);

    ESP_LOGI(TAG, "%s: startBit=%u, length=%u",
             vehicleVelocity.name, vehicleVelocity.startBit, vehicleVelocity.length);

    int64_t lastTx = esp_timer_get_time();
    uint32_t counter = 0;

    while (true)
    {
        receiveTest();

        int64_t now = esp_timer_get_time();

        if (now - lastTx >= static_cast<int64_t>(CAN_PERIOD_MS) * 1000)
        {
            lastTx = now;
            transmitTest(counter++);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
