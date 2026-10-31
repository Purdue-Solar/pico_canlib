#include "pico_canlib.hpp"
#include "artemis_canid.hpp"
#include <stdio.h>
#include "Arduino.h"
#include <stdbool.h>
#include <string.h>

#define TX 1
#define RX 0

#define TX_OR_RX TX
pico_canlib can = pico_canlib();
pico_canlib::status errorCode;
uint8_t SOC[8] = {2, 2, 3, 4, 5, 6, 7};
uint8_t status;
#if TX_OR_RX == TX
void setup()
{

    sleep_ms(1000);
    Serial.println("wsp");
    fprintf(stdout, "Start\n");

    // init canlib

    errorCode = can.init();
    fprintf(stdout, "Init Code %d\n", errorCode);
    if (errorCode != pico_canlib::status::SUCCESS)
    {
        fprintf(stdout, "Failed Startup\n");
    }

    // message

    // return 0;
}
void loop()
{
    // transmits to tx buffer 2 in mcp2515
    errorCode = can.transmitCAN(XL2515::TX_BUFFER_SEL::TX2, 0x203, false, SOC, 8, XL2515::PRIORITY::Highest);
    // error
    if (errorCode != pico_canlib::status::SUCCESS)
    {
        Serial.println("Failed to Transmit. Error Code #");
    }
    // success
    else
    {
        Serial.println("Successfully Transmitted. Code #%d\n");
        asm volatile("nop \n nop \n nop \n nop \n nop \n nop \n");
        // pg70 of mcp2515 datasheet
        errorCode = can.checkStatus(&status);
        // if (errorCode & )
        if (errorCode != pico_canlib::status::SUCCESS)
        {
            fprintf(stdout, "Failed to fetch status byte Error Code #%d\n", errorCode);
        }
        // 8 = tx0, 32 = tx1, 128 = tx2
        fprintf(stdout, "Status Buffer: %d\n", status);
    }
    sleep_ms(1000);
    SOC[7] = (SOC[7] + 1) % 256;
}
#else
void CAN_isr();
void CAN_irq_init();

volatile bool data_available = false;

void CAN_isr()
{
    if (gpio_get_irq_event_mask(8) & GPIO_IRQ_EDGE_FALL)
    {
        gpio_acknowledge_irq(8, GPIO_IRQ_EDGE_FALL);
        data_available = true;
    }
}

void CAN_irq_init()
{
    // 8 is gpio int
    gpio_init(8);
    gpio_set_dir(8, false);
    gpio_pull_up(8);
    irq_set_exclusive_handler(IO_IRQ_BANK0, CAN_isr);
    gpio_set_irq_enabled(8, GPIO_IRQ_EDGE_FALL, true);
    irq_set_enabled(IO_IRQ_BANK0, true);
}

void setup()
{

    // stdio_init_all();
    sleep_ms(2000);
    fprintf(stdout, "Start\n");
    pico_canlib can = pico_canlib();
    if (can.init() != pico_canlib::status::SUCCESS)
    {
        printf("CAN Init Failed\n");
    }

    CAN_irq_init();
    printf("irq set up\n");
}

void loop()
{
    uint8_t buffer[13];

    if (data_available)
    {
        data_available = false;
        can.receiveCAN(buffer, 4, 8);
        printf("id: %d\n", buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3]);
        printf("dlc: %d\n", buffer[4]);
        printf("data: ");
        for (int i = 0; i < 8; i++)
        {
            printf("%d ", buffer[5 + i]);
        }
        printf("\n");
    }
    sleep_ms(10);
}
#endif