#pragma once

#include <stdio.h>

class XL2515{
    public:
        enum class SPI_INSTR_XL {
            RESET        = 0xC0,
            READ         = 0x03,
            READ_RX_BUFF = 0x90,
            WRITE        = 0x02,
            LOAD_TX_BUFF = 0x40, 
            RTS          = 0x80,
            READ_STATUS  = 0xA0,
            RX_STATUS    = 0xB0,
            BIT_MODIFY   = 0x05
        };

        enum class dir_RX_Address{
            RXB0_ID,
            RXB0_DAT,
            RXB1_ID,
            RXB1_DAT
        };

        enum class dir_TX_Address{
            TXB0_ID,
            TXB0_DAT,
            TXB1_ID,
            TXB1_DAT,
            TXB2_ID,
            TXB2_DAT
        };

        enum class RX_Address{
            RXB0_ID = 0x61,
            RXB0_DAT = 0x66,
            RXB1_ID = 0x71,
            RXB1_DAT = 0x76
        };

        enum class TX_Address{
            TXB0_ID  = 0x31,
            TXB0_DAT = 0x36,
            TXB1_ID  = 0x41,
            TXB1_DAT = 0x46,
            TXB2_ID  = 0x51,
            TXB2_DAT = 0x56
        };

        struct write_buffer{
            SPI_INSTR_XL instruction; //SPI_INSTR_XL
            uint8_t addr;
            uint8_t * payload;
        };

        struct read_buffer{
            SPI_INSTR_XL instruction;
            uint8_t addr;
            uint8_t * payload;
        };

        struct dir_read_rx_buffer{
            SPI_INSTR_XL instruction;
            uint8_t * payload;
        };
        
        struct dir_load_tx_buffer{
            SPI_INSTR_XL instruction;
            uint8_t * payload;
        };

        struct dri_read_rx_buffer{
            SPI_INSTR_XL instruction;
            uint8_t * payload;
        };

    private:
        
};



