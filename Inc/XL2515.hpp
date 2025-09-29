#pragma once
#include <stdio.h>

#define XL2515_BAUDRATE 1000000

class XL2515{
    public:
        /// @brief XL2515 SPI Communication Protocol
        enum SPI_INSTR_XL {
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

        /// @brief Mask for READ_RX_BUFF
        enum dir_RX_Address{
            dir_RXB0_ID,
            dir_RXB1_ID,
            dir_RXB0_DAT,
            dir_RXB1_DAT
        };

        /// @brief Mask for LOAD_TX_BUFF
        enum dir_TX_Address{
            dir_TXB0_ID,
            dir_TXB1_ID,
            dir_TXB2_ID,
            dir_TXB0_DAT,
            dir_TXB1_DAT,
            dir_TXB2_DAT
        };

        /// @brief Address mask for READ
        enum RX_Address{
            RXB0_ID = 0x61,
            RXB0_DAT = 0x66,
            RXB1_ID = 0x71,
            RXB1_DAT = 0x76
        };

        /// @brief Address mask for WRITE
        enum TX_Address{
            TXB0_ID  = 0x31,
            TXB0_DAT = 0x36,
            TXB1_ID  = 0x41,
            TXB1_DAT = 0x46,
            TXB2_ID  = 0x51,
            TXB2_DAT = 0x56
        };

        /// @brief TX_Buffer Selection Mask
        enum TX_SEL{
            TX0 = 1,
            TX1 = 2,
            TX2 = 4
        };

        /// @brief RX_Buffer Selection Mask
        enum RX_SEL{
            RX0 = 0,
            RX1 = 1,
        };

        /// @brief WRITE SPI payload struct
        struct write_buffer{
            uint8_t instruction; //SPI_INSTR_XL
            uint8_t addr;
            uint8_t * payload;
        };

        /// @brief READ SPI payload struct
        struct read_buffer{
            uint8_t instruction;
            uint8_t addr;
            uint8_t * payload;
        };

        /// @brief READ_RX_BUFFER SPI payload struct
        struct dir_read_rx_buffer{
            uint8_t instruction;
        };
        
        /// @brief LOAD_TX_BUFFER SPI payload struct
        struct dir_load_tx_buffer{
            uint8_t instruction;
            uint8_t * payload;
        };

    private:
        
};



