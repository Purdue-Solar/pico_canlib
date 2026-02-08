#include "pico_canlib.hpp"
#include "artemis_canid.hpp"
#include <stdio.h>
#include <stdbool.h>

int main(void){
    stdio_usb_init();
    sleep_ms(1000);
    fprintf(stdout, "Start\n");
    pico_canlib can = pico_canlib();
    pico_canlib::status errorCode;
    errorCode = can.init();
    fprintf(stdout, "Init Code %d\n", errorCode);
    if (errorCode != pico_canlib::status::SUCCESS){
        fprintf(stdout, "Failed Startup\n");
    }
    uint8_t SOC[8] = {0,0,0,100,0,0,0,0};
    uint8_t status;
    while (true){
        errorCode = can.transmitCAN(XL2515::TX_BUFFER_SEL::TX0, artemis_canid::tempAndSOC, false, SOC, 8, XL2515::PRIORITY::Highest);
        if (errorCode != pico_canlib::status::SUCCESS){
            fprintf(stdout, "Failed to Transmit. Error Code #%d\n", errorCode);
        }
        else{
            fprintf(stdout, "Successfully Transmitted. Code #%d\n", errorCode);
            asm volatile("nop \n nop \n nop \n nop \n nop \n nop \n");
            errorCode = can.checkStatus(&status);
            // if (errorCode & )
            if (errorCode != pico_canlib::status::SUCCESS){
                fprintf(stdout, "Failed to fetch status byte Error Code #%d\n", errorCode);
            }
            fprintf(stdout, "Status Buffer: %d\n", status);
        }
        sleep_ms(1000);

    }
    // while (true){
    //     sleep_ms(500);
    // }
    
    return 0;
}