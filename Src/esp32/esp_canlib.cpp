#include "esp_canlib.h"

#include <cstring>

esp_canlib::esp_canlib(int baud, int txPin, int rxPin)
    : canlib(baud), txPin(txPin), rxPin(rxPin)
{
}

esp_canlib::~esp_canlib()
{
    irqTaskRunning = false;

    while (irqTaskHandle != nullptr)
    {
        vTaskDelay(1);
    }

    if (initialized)
    {
        twai_stop();
        twai_driver_uninstall();
        initialized = false;
    }
}

bool esp_canlib::getTiming(twai_timing_config_t &config)
{
    switch (getBaud())
    {
    case 25000:
        config = TWAI_TIMING_CONFIG_25KBITS();
        return true;
    case 50000:
        config = TWAI_TIMING_CONFIG_50KBITS();
        return true;
    case 100000:
        config = TWAI_TIMING_CONFIG_100KBITS();
        return true;
    case 125000:
        config = TWAI_TIMING_CONFIG_125KBITS();
        return true;
    case 250000:
        config = TWAI_TIMING_CONFIG_250KBITS();
        return true;
    case 500000:
        config = TWAI_TIMING_CONFIG_500KBITS();
        return true;
    case 1000000:
        config = TWAI_TIMING_CONFIG_1MBITS();
        return true;
    default:
        return false;
    }
}

status esp_canlib::init()
{
    if (initialized)
    {
        return status::SUCCESS;
    }

    twai_timing_config_t timing;

    if (!getTiming(timing))
    {
        return status::INIT_ERROR;
    }

    twai_general_config_t general = TWAI_GENERAL_CONFIG_DEFAULT(
        static_cast<gpio_num_t>(txPin),
        static_cast<gpio_num_t>(rxPin),
        TWAI_MODE_NORMAL);

    general.tx_queue_len = 5;
    general.rx_queue_len = 16;
    general.alerts_enabled = TWAI_ALERT_NONE;

    twai_filter_config_t filter = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    if (twai_driver_install(&general, &timing, &filter) != ESP_OK)
    {
        return status::INIT_ERROR;
    }

    if (twai_start() != ESP_OK)
    {
        twai_driver_uninstall();
        return status::INIT_ERROR;
    }

    initialized = true;

    return status::SUCCESS;
}

status esp_canlib::init_irq()
{
    if (!initialized)
    {
        return status::INIT_ERROR;
    }

    if (irqInitialized)
    {
        return status::SUCCESS;
    }

    if (twai_reconfigure_alerts(TWAI_ALERT_RX_DATA, nullptr) != ESP_OK)
    {
        return status::INIT_ERROR;
    }

    irqTaskRunning = true;
    rxPending = false;

    TaskHandle_t task = nullptr;

    BaseType_t result = xTaskCreate(
        irqTaskEntry,
        "can_irq",
        3072,
        this,
        tskIDLE_PRIORITY + 2,
        &task);

    if (result != pdPASS)
    {
        irqTaskRunning = false;
        twai_reconfigure_alerts(TWAI_ALERT_NONE, nullptr);
        return status::INIT_ERROR;
    }

    irqTaskHandle = task;
    irqInitialized = true;

    return status::SUCCESS;
}

void esp_canlib::irqTaskEntry(void *arg)
{
    esp_canlib *instance = static_cast<esp_canlib *>(arg);

    while (instance->irqTaskRunning)
    {
        uint32_t alerts = 0;

        esp_err_t result = twai_read_alerts(
            &alerts,
            pdMS_TO_TICKS(20));

        if (result == ESP_OK && (alerts & TWAI_ALERT_RX_DATA))
        {
            instance->can_isr();
        }
    }

    instance->irqTaskHandle = nullptr;
    vTaskDelete(nullptr);
}

void esp_canlib::can_isr()
{
    // Called by the CAN alert task, not directly by the hardware ISR.
    // The TWAI driver has already queued the received message.
    rxPending = true;
}

status esp_canlib::filtersAndMasks(const uint8_t *data, uint8_t length, uint32_t addr)
{
    // MCP2515 register addresses do not apply to ESP32 TWAI.

    (void)data;
    (void)length;
    (void)addr;

    return status::SET_CONTROL_BITS_ERROR;
}

status esp_canlib::reset()
{
    if (!initialized)
    {
        return status::RESET_INIT_ERROR;
    }

    if (twai_stop() != ESP_OK)
    {
        return status::RESET_ERROR;
    }

    if (twai_start() != ESP_OK)
    {
        return status::RESET_ERROR;
    }

    rxPending = false;

    return status::SUCCESS;
}

status esp_canlib::transmitCAN(uint32_t can_id, uint8_t *data_buffer, uint8_t data_length)
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

    twai_message_t message = {};

    message.identifier = can_id;
    message.extd = 0;
    message.rtr = 0;
    message.data_length_code = data_length;

    if (data_length > 0)
    {
        std::memcpy(message.data, data_buffer, data_length);
    }

    esp_err_t result = twai_transmit(&message, pdMS_TO_TICKS(100));

    if (result == ESP_OK)
    {
        return status::SUCCESS;
    }

    if (result == ESP_ERR_TIMEOUT)
    {
        return status::TX_STATUS_STALL;
    }

    return status::TX_STATUS_ERROR;
}

status esp_canlib::receiveCAN(uint8_t *buffer, uint8_t idSize, uint8_t bufferSize)
{
    if (!initialized || buffer == nullptr || idSize != 4 || bufferSize != 8)
    {
        return status::RX_PAYLOAD_ERROR;
    }

    twai_message_t message = {};

    esp_err_t result = twai_receive(&message, 0);

    if (result == ESP_ERR_TIMEOUT)
    {
        rxPending = false;
        return status::NO_NEW_MESSAGE;
    }

    if (result != ESP_OK)
    {
        return status::RX_STATUS_ERROR;
    }

    if (message.extd || message.rtr || message.identifier > 0x7FF)
    {
        return status::RX_ID_ERROR;
    }

    if (message.data_length_code > 8)
    {
        return status::RX_PAYLOAD_ERROR;
    }

    std::memset(buffer, 0, 13);

    uint32_t id = message.identifier;

    buffer[0] = static_cast<uint8_t>(id);
    buffer[1] = static_cast<uint8_t>(id >> 8);
    buffer[2] = static_cast<uint8_t>(id >> 16);
    buffer[3] = static_cast<uint8_t>(id >> 24);
    buffer[4] = message.data_length_code;

    if (message.data_length_code > 0)
    {
        std::memcpy(buffer + 5, message.data, message.data_length_code);
    }

    twai_status_info_t info = {};

    if (twai_get_status_info(&info) == ESP_OK)
    {
        rxPending = info.msgs_to_rx > 0;
    }

    return status::SUCCESS;
}

status esp_canlib::checkStatus(uint8_t *statusByte)
{
    if (!initialized || statusByte == nullptr)
    {
        return status::STATUS_ERROR;
    }

    twai_status_info_t info = {};

    if (twai_get_status_info(&info) != ESP_OK)
    {
        return status::STATUS_ERROR;
    }

    *statusByte = 0;

    if (info.state == TWAI_STATE_RUNNING)
    {
        *statusByte |= 0x01;
    }

    if (info.state == TWAI_STATE_STOPPED)
    {
        *statusByte |= 0x02;
    }

    if (info.state == TWAI_STATE_BUS_OFF)
    {
        *statusByte |= 0x04;
    }

    if (info.state == TWAI_STATE_RECOVERING)
    {
        *statusByte |= 0x08;
    }

    if (info.msgs_to_rx > 0)
    {
        *statusByte |= 0x10;
    }

    if (info.msgs_to_tx > 0)
    {
        *statusByte |= 0x20;
    }

    return status::SUCCESS;
}