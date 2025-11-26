#pragma once

#include <stdint.h>
#include <stddef.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <iostream>
#include <string>


#define XL2515_BAUDRATE 1000000

/// @brief XL2515 Commands
class XL2515{
    public:
        /// @brief XL2515 SPI Communication Protocol
        enum class SPI_INSTR_XL : uint8_t {
            RESET        = 0xC0,
            READ         = 0x03,
            READ_RX_BUFF = 0x90,
            WRITE        = 0x02,
            LOAD_TX_BUFF = 0x40, 
            RTS          = 0x80,
            READ_STATUS  = 0xA0,
            RX_STATUS    = 0xB0,
            BIT_MODIFY   = 0x05,
            REQTS        = 0x80
        };

        /// @brief Mask for READ_RX_BUFF
        enum class dir_RX_Address : uint8_t {
            dir_RXB0_ID,
            dir_RXB1_ID,
            dir_RXB0_DAT,
            dir_RXB1_DAT
        };

        /// @brief Mask for LOAD_TX_BUFF
        enum class dir_TX_Address : uint8_t {
            dir_TXB0_ID,
            dir_TXB1_ID,
            dir_TXB2_ID,
            dir_TXB0_DAT,
            dir_TXB1_DAT,
            dir_TXB2_DAT
        };

        /// @brief Address mask for READ
        enum class RX_Address : uint8_t {
            RXB0_ID = 0x61,
            RXB0_DAT = 0x66,
            RXB1_ID = 0x71,
            RXB1_DAT = 0x76
        };

        /// @brief Address mask for WRITE
        enum class TX_Address : uint8_t {
            TXB0_ID  = 0x31,
            TXB0_DAT = 0x36,
            TXB1_ID  = 0x41,
            TXB1_DAT = 0x46,
            TXB2_ID  = 0x51,
            TXB2_DAT = 0x56
        };

        /// @brief TX_Buffer Selection Mask
        enum class TX_BUFFER_SEL : uint8_t {
            TX0 = 0,
            TX1 = 2,
            TX2 = 4
        };

        /// @brief RX_Buffer Selection Mask
        enum class RX_BUFFER_SEL : uint8_t {
            RX0 = 0,
            RX1 = 1,
        };


        static constexpr uint8_t write_buffer_len = 14;
        /// @brief WRITE SPI payload struct
        struct write_buffer{
            uint8_t id_TXBuffer_instr = (uint8_t) XL2515::SPI_INSTR_XL::LOAD_TX_BUFF;
            uint32_t can_id;   // 29 bit but obviously not gonna fit so define as 32 bit, there is a total of 5 bytes for ID within chip memory so should be fine
            uint8_t payload_TXBuffer_instr = (uint8_t) XL2515::SPI_INSTR_XL::LOAD_TX_BUFF;
            uint8_t * payload; //Length 8 bytes
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
        // struct dir_load_tx_buffer{
        //     uint8_t * instruction;
        //     uint8_t * payload;
        // };

    private:
        
};

/// @brief RP2350 functions to communicate with XL2515 CAN tranciever
class pico_canlib
{
public:
    

    /// @brief Class constructor
    /// @param gpioInt GPIO Interrupt
    /// @param miso Master In Slave Out (MISO) pin
    /// @param mosi Master Out Slave In (MOSI) pin
    /// @param cs   Chip Select Pin
    /// @param sck  Serial CLK pin
    /// @param spi_hw SPI peripheral address
    pico_canlib(uint8_t gpioInt = 8, uint8_t miso = 12, uint8_t mosi =11, uint8_t cs = 9, uint8_t sck = 10, spi_inst_t * spi_hw = spi0) : in_intGPIO(gpioInt), in_miso(miso), in_mosi(mosi), in_cs(cs), in_sck(sck), in_spi_hw(spi_hw) {};
    // ~pico_canlib();

    /// @brief Error Codes when using CAN, only use these if CAN is used
    enum class status : uint8_t {
        SUCCESS = 0,
        TX_ID_COMMAND_ERROR = 1,
        TX_PAYLOAD_COMMAND_ERROR = 2,
        TX_ID_ERROR = 3,
        TX_PAYLOAD_ERROR = 4,
        RX_ID_ERROR = 5,
        RX_PAYLOAD_ERROR = 6,
        RX_STATUS_ERROR = 7,
        RX_STATUS_DONE = 8,
        RX_STATUS_STALL = 9,
        TX_STATUS_ERROR = 10,
        TX_STATUS_DONE = 11,
        TX_STATUS_STALL = 12,
        RESET_ERROR = 13,
        RESET_INIT_ERROR = 14,
        REQUESTTS_ERROR = 15
    }; //Don't ask me why I label them even though this is an enum class. It is for ease of debugging status

    /// @brief Initialized spi ports and reset XL2515 Configuration
    status init();

    /// @brief Send SPI request to send CAN messages
    /// @param TX_SEL TX Buffer select (0, 1, or 2)
    /// @param id    29 bits extended ids as bytes array
    /// @param TX_buffer  Payload bytes array
    /// @param length Length in bytes of payload
    /// @return True if SPI request was successful sent
    status transmitCAN(XL2515::TX_BUFFER_SEL TX_SEL, uint32_t can_id, uint8_t idSize, uint8_t* TX_buffer, uint8_t length);
    
    /// @brief Send SPI request to recieve CAN messages
    /// @param RX_ID RX Buffer select
    /// @param buffer Bytes array to save payload
    /// @param idSize Length in bytes of id
    /// @param bufferSize Length in bytes of payload
    /// @return True if SPI request was successful sent
    status receiveCAN(uint8_t RX_ID, uint8_t* buffer, uint8_t idSize, uint8_t bufferSize);
    
    /// @brief Reset XL2515 Configuration
    status reset();
    
    /// @brief Check if RX has anything in it
    /// @param status Return status byte (XL2515 Specifics. Check datasheet)
    /// @return Status of all Buffer
    status checkStatus(uint8_t * status);
private:
    ///Idk LED indicator or smth, default LED pin is 25 on Pico 2. Thinking of implementing it but it won't fix my problem just excessive. Cool feature to have tho
    bool isLED;
    uint in_LEDPin;
    uint8_t in_intGPIO;
    uint8_t in_miso;
    uint8_t in_mosi;
    uint8_t in_cs;
    uint8_t in_sck;
    spi_inst_t * in_spi_hw;

    /// @brief Device Request To Send Signal
    /// @param buffer Determine which TX buffer to use
    status requestTS(uint8_t buffer);
};
