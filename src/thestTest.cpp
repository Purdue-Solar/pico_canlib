// #include "pico_canlib.hpp"
// #include "artemis_canid.hpp"
// #include "stdio.h"
// #include <stdbool.h>
// #include <string.h>

// int main()
// {
//     stdio_init_all();
//     sleep_ms(2000);

//     printf("\n=== XL2515 CAN RX TEST ===\n");

//     pico_canlib can;

//     if (can.init() != pico_canlib::status::SUCCESS)
//     {
//         printf("CAN init FAILED\n");
//         while (1)
//             tight_loop_contents();
//     }

//     printf("CAN init OK\n");

//     // Accept all valid CAN messages.
//     can.setByte(0x60, XL2515::IN_ADDR::RXB0CTRL);
//     can.setByte(0x60, XL2515::IN_ADDR::RXB1CTRL);

//     uint8_t value;

//     can.getByte(&value, XL2515::IN_ADDR::RXB0CTRL);
//     printf("RXB0CTRL = 0x%02X\n", value);

//     can.getByte(&value, XL2515::IN_ADDR::RXB1CTRL);
//     printf("RXB1CTRL = 0x%02X\n", value);

//     printf("\nWaiting for CAN frames...\n");

//     while (true)
//     {
//         uint8_t canintf;

//         // Read CANINTF
//         can.getByte(&canintf, XL2515::IN_ADDR::CANINTF);

//         if (canintf & 0x01)
//         {
//             printf("\nRXB0 received!\n");

//             uint8_t buffer[13];

//             // READ RX BUFFER 0
//             uint8_t instr =
//                 (uint8_t)XL2515::SPI_INSTR_XL::READ_RX_BUFF;

//             gpio_put(can.in_cs, 0);

//             spi_write_blocking(can.in_spi_hw, &instr, 1);

//             spi_read_blocking(
//                 can.in_spi_hw,
//                 0,
//                 buffer,
//                 13
//             );

//             gpio_put(can.in_cs, 1);

//             // MCP2515 RX buffer:
//             //
//             // buffer[0] = SIDH
//             // buffer[1] = SIDL
//             // buffer[2] = EID8
//             // buffer[3] = EID0
//             // buffer[4] = DLC
//             // buffer[5..12] = DATA

//             uint16_t id =
//                 ((uint16_t)buffer[0] << 3) |
//                 ((uint16_t)buffer[1] >> 5);

//             uint8_t dlc = buffer[4] & 0x0F;

//             printf("ID  : 0x%03X (%d)\n", id, id);
//             printf("DLC : %d\n", dlc);

//             printf("DATA: ");

//             for (int i = 0; i < dlc && i < 8; i++)
//             {
//                 printf("%02X ", buffer[5 + i]);
//             }

//             printf("\n");

//             // Clear RX0 interrupt flag
//             can.modifiedBit(
//                 0x00,
//                 (uint8_t)XL2515::IN_ADDR::CANINTF,
//                 0x01
//             );
//         }

//         if (canintf & 0x02)
//         {
//             printf("\nRXB1 received!\n");

//             uint8_t buffer[13];

//             // READ RX BUFFER 1
//             uint8_t instr =
//                 (uint8_t)XL2515::SPI_INSTR_XL::READ_RX_BUFF |
//                 (1 << 2);

//             gpio_put(can.in_cs, 0);

//             spi_write_blocking(can.in_spi_hw, &instr, 1);

//             spi_read_blocking(
//                 can.in_spi_hw,
//                 0,
//                 buffer,
//                 13
//             );

//             gpio_put(can.in_cs, 1);

//             uint16_t id =
//                 ((uint16_t)buffer[0] << 3) |
//                 ((uint16_t)buffer[1] >> 5);

//             uint8_t dlc = buffer[4] & 0x0F;

//             printf("ID  : 0x%03X (%d)\n", id, id);
//             printf("DLC : %d\n", dlc);

//             printf("DATA: ");

//             for (int i = 0; i < dlc && i < 8; i++)
//             {
//                 printf("%02X ", buffer[5 + i]);
//             }

//             printf("\n");

//             // Clear RX1 interrupt flag
//             can.modifiedBit(
//                 0x00,
//                 (uint8_t)XL2515::IN_ADDR::CANINTF,
//                 0x02
//             );
//         }

//         sleep_ms(10);
//     }
// }