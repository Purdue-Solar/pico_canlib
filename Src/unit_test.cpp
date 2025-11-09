#include "pico_canlib.hpp"
#include "artemis_canid.hpp"

int main(void){
    stdio_init_all();
    pico_canlib can = pico_canlib();
    can.init();
    uint8_t SOC[8] = {0,0,0,100,0,0,0,0};
    while (true){
        can.transmitCAN(0, (uint8_t *) &artemis_canid::tempSOC, 4, SOC, 8);
        while (!can.checkRXStatus()){
            fprintf(stdout, "RX Pending\n");
        }
        fprintf(stdout, "RX Done\n");
    }
    

    return 0;
}