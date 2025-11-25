#include "pico_canlib.hpp"
#include "artemis_canid.hpp"
#include <stdio.h>

int main(void){
    stdio_init_all();
    pico_canlib can = pico_canlib();
    can.init();
    uint8_t SOC[8] = {0,0,0,100,0,0,0,0};
    while (true){
        pico_canlib::status errorCode = can.transmitCAN(0, (uint8_t *) &artemis_canid::tempAndSOC, 4, SOC, 8);
        if (errorCode != pico_canlib::status::SUCCESS){
            fprintf(stderr, "Failed to Transmit. Error Code #%d\n", errorCode);
        }
        else{
            fprintf(stdout, "Successfully Transmitted. Code #%d\n", errorCode);
            asm volatile("nop \n nop \n nop \n nop \n nop \n nop \n");
        }
    }
    
    return 0;
}