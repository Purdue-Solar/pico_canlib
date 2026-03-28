#include "pico_canlib.hpp"
#include "artemis_canid.hpp"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hardware/gpio.h"

#define TX_OR_RX 1 // tx = 0, rx = 1

#if TX_OR_RX == 0
int main(void)
{
    stdio_init_all();
    sleep_ms(1000);
    fprintf(stdout, "Start\n");
    pico_canlib can = pico_canlib();
    pico_canlib::status errorCode;
    errorCode = can.init();
    fprintf(stdout, "Init Code %d\n", errorCode);
    if (errorCode != pico_canlib::status::SUCCESS)
    {
        fprintf(stdout, "Failed Startup\n");
    }
    // message
    uint8_t SOC[8] = {0};
    uint8_t status;
    while (true)
    {
        // transmits to tx buffer 2 in mcp2515
        errorCode = can.transmitCAN(XL2515::TX_BUFFER_SEL::TX2, artemis_canid::tempAndSOC, false, SOC, 8, XL2515::PRIORITY::Highest);
        if (errorCode != pico_canlib::status::SUCCESS)
        {
            fprintf(stdout, "Failed to Transmit. Error Code #%d\n", errorCode);
        }
        else
        {
            fprintf(stdout, "Successfully Transmitted. Code #%d\n", errorCode);
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

    return 0;
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

int main(void)
{
    uint8_t buffer[13];

    stdio_init_all();
    sleep_ms(2000);
    fprintf(stdout, "Start\n");
    pico_canlib can = pico_canlib();
    if (can.init() != pico_canlib::status::SUCCESS)
    {
        printf("CAN Init Failed\n");
    }

    CAN_irq_init();
    printf("irq set up\n");

    for (;;)
    {
        // if (can.receiveCAN(buffer, 4, 8) == pico_canlib::status::SUCCESS)
        // {
        //     printf("id: %d\n", buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3]);
        //     printf("dlc: %d\n", buffer[4]);
        //     printf("data: ");
        //     for (int i = 0; i < 8; i++)
        //     {
        //         printf("%d ", buffer[5 + i]);
        //     }
        //     printf("\n");
        //     data_available = false;
        // }
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

    return 0;
}

#endif