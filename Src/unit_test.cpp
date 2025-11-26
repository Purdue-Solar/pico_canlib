#include "pico_canlib.hpp"
#include "artemis_canid.hpp"
#include <stdio.h>

int main(void){
    stdio_init_all();
    pico_canlib can = pico_canlib(25);
    can.init();
    uint8_t SOC[8] = {0,0,0,100,0,0,0,0};
    uint8_t status;
    while (true){
        pico_canlib::status errorCode = can.transmitCAN(XL2515::TX_BUFFER_SEL::TX0, (uint8_t *) &artemis_canid::tempAndSOC, 4, SOC, 8);
        if (errorCode != pico_canlib::status::SUCCESS){
            fprintf(stderr, "Failed to Transmit. Error Code #%d\n", errorCode);
        }
        else{
            fprintf(stdout, "Successfully Transmitted. Code #%d\n", errorCode);
            asm volatile("nop \n nop \n nop \n nop \n nop \n nop \n");
            errorCode = can.checkStatus(&status);
            if (errorCode != pico_canlib::status::SUCCESS){
                fprintf(stderr, "Failed to fetch status byte Error Code #%d\n", errorCode);
            }
            fprintf(stdout, "Status Buffer: %d\n", status);
        }
    }
    
    return 0;
}