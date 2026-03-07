#include "pico_canlib.hpp"
#include "artemis_canid.hpp"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TX_OR_RX 1 // tx = 0, rx = 1

#if TX_OR_RX == 0
int main(void)
{
    stdio_init_all();
    sleep_ms(1000);
    fprintf(stdout, "Start\n");

    // init canlib
    pico_canlib can = pico_canlib();
    pico_canlib::status errorCode;
    errorCode = can.init();
    fprintf(stdout, "Init Code %d\n", errorCode);
    if (errorCode != pico_canlib::status::SUCCESS)
    {
        fprintf(stdout, "Failed Startup\n");
    }

    // message
    uint8_t SOC[8] = {6, 7, 6, 7, 6, 8, 7, 6};
    // status, 8 if tx0, 32 if tx1, 128 if tx2
    uint8_t status;

    // main loop
    while (true)
    {
        // transmits to tx buffer 2 in mcp2515
        errorCode = can.transmitCAN(XL2515::TX_BUFFER_SEL::TX2, artemis_canid::tempAndSOC, false, SOC, 8, XL2515::PRIORITY::Highest);
        // error
        if (errorCode != pico_canlib::status::SUCCESS)
        {
            fprintf(stdout, "Failed to Transmit. Error Code #%d\n", errorCode);
        }
        // success
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
        sleep_ms(1);
    }

    return 0;
}
#else
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

    uint8_t buffer[13];
    uint32_t id;
    uint8_t st;
    while (true)
    {
        if (can.receiveCAN(buffer, 4, 8) == pico_canlib::status::SUCCESS)
        {
            printf("id: %d\n", buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3]);
            printf("dlc: %d\n", buffer[4]);
            printf("data: ");
            for (int i = 0; i < 8; i++)
            {
                printf("%d ", buffer[5 + i]);
            }
            printf("\n");
        }
        else
        {
            printf("no new message\n");
        }
        sleep_ms(1);
    }
}
#endif