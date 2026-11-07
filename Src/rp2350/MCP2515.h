#pragma once

#include <cstdint>

/// @brief XL2515 Commands
class XL2515
{
public:
    static const uint32_t XL2515_BAUDRATE = 10000000;

    static const uint8_t NORMAL_MODE = 0x00;
    static const uint8_t LOOPBACK_MODE = 0x40;
    static const uint8_t INTE_EN = 0x03; // 0x03 for rxb0 and rxb1 interrupt enable
    static const uint8_t NORMAL_CNF1 = 0x00;
    static const uint8_t NORMAL_CNF2 = 0xBC;
    static const uint8_t NORMAL_CNF3 = 0x01;

    enum class PRIORITY : uint8_t
    {
        Lowest = 0,
        Low,
        High,
        Highest
    };

    enum class IN_ADDR : uint8_t
    {
        // Filter/mask register blocks. Contiguous within each block, but the
        // blocks themselves are split by the CANSTAT/CANCTRL mirror registers
        // at 0x0C-0x0F (and 0x1C-0x1F) -- see filtersAndMasks() call sites in
        // init(). Each filter/mask is 4 bytes (SIDH, SIDL, EID8, EID0).
        RXF0SIDH = 0x00, // filters 0-2 (RXB0): 0x00-0x0B, 12 bytes
        RXF3SIDH = 0x10, // filters 3-5 (RXB1): 0x10-0x1B, 12 bytes
        RXM0SIDH = 0x20, // masks 0-1 (RXB0/RXB1): 0x20-0x27, 8 bytes
        CANINTE = 0x2B,
        CANCTRL = 0x0F,
        CNF1 = 0x2A,
        CNF2 = 0x29,
        CNF3 = 0x28,
        RXB0CTRL = 0x60,
        RXB1CTRL = 0x70,
        TXBxCTRL = 0x30,
        TXBxSIDH = 0x31,
        TXBxSIDL = 0x32,
        TXBxEID8 = 0x33,
        TXBxEID0 = 0x34,
        TXBxDLC = 0x35,
        CANINTF = 0x2C, // interrupt flag register (added for clearing RX flags)
        CANSTAT = 0x0E, // status register; OPMOD (bits 7:5) reports actual mode
    };

    // RXB0CTRL: RXM<1:0> = 00 (bits 6:5) means "use filters/masks" -- with all
    // masks zeroed below that accepts every valid frame without resorting to
    // RXM=11, which the datasheet (S4.2.2) flags as debug-only. BUKT (bit 2)
    // = 1 enables RXB0 -> RXB1 rollover so a full RXB0 doesn't drop the next
    // frame (finding M-10; effective RX depth 1 -> 2).
    static const uint8_t RXB0CTRL_RXALL_BUKT = 0x04;
    // RXB1CTRL: RXM<1:0> = 00, same reasoning; RXB1 has no BUKT bit.
    static const uint8_t RXB1CTRL_RXALL = 0x00;

    /// @brief XL2515 SPI Communication Protocol
    enum class SPI_INSTR_XL : uint8_t
    {
        RESET = 0xC0,
        READ = 0x03,
        READ_RX_BUFF = 0x90,
        WRITE = 0x02,
        LOAD_TX_BUFF = 0x40, // might need to change to 0x41, 0x41 sends to data buffer, not command buffer
        RTS = 0x80,
        READ_STATUS = 0xA0,
        RX_STATUS = 0xB0,
        BIT_MODIFY = 0x05,
        REQTS = 0x80
    };

    /// @brief Mask for READ_RX_BUFF
    enum class dir_RX_Address : uint8_t
    {
        dir_RXB0_ID,
        dir_RXB1_ID,
        dir_RXB0_DAT,
        dir_RXB1_DAT
    };

    /// @brief Mask for LOAD_TX_BUFF
    enum class dir_TX_Address : uint8_t
    {
        dir_TXB0_ID,
        dir_TXB1_ID,
        dir_TXB2_ID,
        dir_TXB0_DAT,
        dir_TXB1_DAT,
        dir_TXB2_DAT
    };

    /// @brief TX_Buffer Selection Mask
    enum class TX_BUFFER_SEL : uint8_t
    {
        TX0 = 0,
        TX1 = 1,
        TX2 = 2,
    };

    /// @brief RX_Buffer Selection Mask
    enum class RX_BUFFER_SEL : uint8_t
    {
        RX0 = 0,
        RX1 = 1,
    };

    static constexpr uint8_t write_buffer_len = 14;
    /// @brief WRITE SPI payload struct
    struct load_tx_buffer
    {
        uint8_t instr;
        uint8_t *payload; // Length 8 bytes
    };

    /// @brief READ SPI payload struct
    struct write_buffer
    {
        uint8_t instruction;
        uint8_t addr;
        uint8_t payload;
    };

    struct bit_modify
    {
        uint8_t instruction = (uint8_t)SPI_INSTR_XL::BIT_MODIFY;
        uint8_t addr;
        uint8_t mask;
        uint8_t payload;
    };

    /// @brief READ_RX_BUFFER SPI payload struct
    struct dir_read_rx_buffer
    {
        uint8_t instruction;
    };

private:
};