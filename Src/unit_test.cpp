#include "pico_canlib.hpp"
#include "artemis_canid.hpp"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TX_OR_RX 0 // tx = 0, rx = 1

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
    uint8_t SOC[8] = {0, 0, 0, 100, 0, 0, 0, 0};
    uint8_t status;
    // while (true){
    errorCode = can.transmitCAN(XL2515::TX_BUFFER_SEL::TX0, artemis_canid::tempAndSOC, false, SOC, 8, XL2515::PRIORITY::Highest);
    if (errorCode != pico_canlib::status::SUCCESS)
    {
        fprintf(stdout, "Failed to Transmit. Error Code #%d\n", errorCode);
    }
    else
    {
        fprintf(stdout, "Successfully Transmitted. Code #%d\n", errorCode);
        asm volatile("nop \n nop \n nop \n nop \n nop \n nop \n");
        errorCode = can.checkStatus(&status);
        // if (errorCode & )
        if (errorCode != pico_canlib::status::SUCCESS)
        {
            fprintf(stdout, "Failed to fetch status byte Error Code #%d\n", errorCode);
        }
        fprintf(stdout, "Status Buffer: %d\n", status);
    }
    sleep_ms(1000);

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

    uint8_t buffer[12];
    uint32_t id;
    uint8_t st;
    while (true)
    {
        can.checkStatus(&st);
        if (st & 0x01)
        {
            if (can.receiveCAN(st, 0x00, buffer, 4, 8) != pico_canlib::status::SUCCESS)
            {
                printf("failed receive rxb0\n");
            }
        }
        if (st & 0x02)
        {
            if (can.receiveCAN(st, 0x01, buffer, 4, 8) != pico_canlib::status::SUCCESS)
            {
                printf("failed receive rxb1\n");
            }
        }
        memcpy(&id, buffer, 4);
        printf("id: %d\n", id);
        printf("data: ");
        for (int i = 0; i < 8; i++)
        {
            printf("%d ", buffer[4 + i]);
        }
        printf("\n");
        sleep_ms(1);
    }
}
#endif