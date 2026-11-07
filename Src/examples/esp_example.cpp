
#include <cstdint>

#include "esp_log.h"
#include "esp_timer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp32/esp_canlib.h"

static constexpr char TAG[] = "CAN_EXAMPLE";

static constexpr uint32_t CAN_ID = 0x123;
static constexpr int CAN_BAUD = 500000;
static constexpr int CAN_TX_PIN = 27;
static constexpr int CAN_RX_PIN = 33;

static esp_canlib espcan1(CAN_BAUD, CAN_TX_PIN, CAN_RX_PIN);
static canlib &can1 = espcan1;

static void processCAN()
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
            ESP_LOGE(TAG, "CAN RX error: %u", static_cast<unsigned>(result));
            break;
        }

        uint32_t id = static_cast<uint32_t>(buffer[0]) |
                      (static_cast<uint32_t>(buffer[1]) << 8) |
                      (static_cast<uint32_t>(buffer[2]) << 16) |
                      (static_cast<uint32_t>(buffer[3]) << 24);

        uint8_t length = buffer[4];

        ESP_LOGI(TAG, "RX ID: 0x%03lX, Length: %u", static_cast<unsigned long>(id), length);

        for (uint8_t i = 0; i < length; i++)
        {
            ESP_LOGI(TAG, "Data[%u]: 0x%02X", i, buffer[5 + i]);
        }
    }
}

static void transmitCAN(uint32_t count)
{
    uint8_t payload[2] = {
        0x01,
        static_cast<uint8_t>(count)};

    status result = can1.transmitCAN(CAN_ID, payload, sizeof(payload));

    if (result == status::SUCCESS)
    {
        ESP_LOGI(TAG, "TX queued: 0x%03lX", static_cast<unsigned long>(CAN_ID));
    }
    else
    {
        ESP_LOGE(TAG, "CAN TX error: %u", static_cast<unsigned>(result));
    }
}

extern "C" void app_main()
{
    ESP_LOGI(TAG, "Initializing ESP32 CAN");

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

    int64_t lastTxUs = esp_timer_get_time();
    uint32_t txCount = 0;

    while (true)
    {
        processCAN();

        int64_t now = esp_timer_get_time();

        if (now - lastTxUs >= 1000000)
        {
            lastTxUs = now;
            transmitCAN(txCount++);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
