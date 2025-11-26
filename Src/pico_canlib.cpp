#include "pico_canlib.hpp"

pico_canlib::status pico_canlib::init(void)
{
    // Initialize SPI port at 1 MHz
    spi_init(in_spi_hw, XL2515_BAUDRATE);
    
    // Set the GPIO functions for the SPI pins
    gpio_set_function(in_miso, GPIO_FUNC_SPI);
    gpio_set_function(in_mosi, GPIO_FUNC_SPI);
    gpio_set_function(in_cs, GPIO_FUNC_SIO);  // CS pin as SIO for manual control
    gpio_set_function(in_sck, GPIO_FUNC_SPI);

    // Set CS pin high (inactive)
    gpio_init(in_cs);
    gpio_set_dir(in_cs, GPIO_OUT);
    gpio_put(in_cs, 1);

    //Reset XL2515 Configuration
    return reset();
}

pico_canlib::status pico_canlib::reset(){
    gpio_put(in_cs, 0);
    uint8_t data = (uint8_t) XL2515::SPI_INSTR_XL::RESET;
    if (spi_write_blocking(in_spi_hw, &data, 8) != 8){
        return pico_canlib::status::RESET_ERROR;
    }
    else {
        return pico_canlib::status::SUCCESS;
    }
    gpio_put(in_cs, 1);
}

pico_canlib::status pico_canlib::requestTS(uint8_t buffer){
    uint8_t data = (uint8_t) XL2515::SPI_INSTR_XL::REQTS | buffer;
    if (spi_write_blocking(in_spi_hw, &data, 1) != 1){
        return pico_canlib::status::REQUESTTS_ERROR;
    }
    else{
        return pico_canlib::status::SUCCESS;
    }
}

pico_canlib::status pico_canlib::checkStatus(uint8_t * status){
    if (spi_read_blocking(in_spi_hw, (uint8_t) XL2515::SPI_INSTR_XL::READ_STATUS, status, 1) != 1){
        return pico_canlib::status::RX_STATUS_ERROR;
    }
    return pico_canlib::status::SUCCESS;
}

pico_canlib::status pico_canlib::transmitCAN(XL2515::TX_BUFFER_SEL TX_SEL, uint32_t can_id, uint8_t idSize, uint8_t* TX_buffer, uint8_t length)
{
    uint8_t TX_ID = (uint8_t) TX_SEL;

    // Pull CS low to select the transceiver
    gpio_put(in_cs, 0);

    // // Send id over SPI
    // uint8_t instruction = (uint8_t) XL2515::SPI_INSTR_XL::LOAD_TX_BUFF | TX_ID;
    // if (spi_write_blocking(in_spi_hw, &instruction, 1) != 1){
    //     gpio_put(in_cs, 1);
    //     return pico_canlib::status::TX_ID_COMMAND_ERROR;
    // }
    
    // if (spi_write_blocking(in_spi_hw, can_id, idSize) != idSize){
    //     gpio_put(in_cs, 1);
    //     return pico_canlib::status::TX_ID_ERROR;
    // }

    // // Send data over SPI
    // instruction = (uint8_t) XL2515::SPI_INSTR_XL::LOAD_TX_BUFF | (TX_ID + 1);
    // if (spi_write_blocking(in_spi_hw, &instruction, 1) != 1){
    //     gpio_put(in_cs, 1);
    //     return pico_canlib::status::TX_PAYLOAD_COMMAND_ERROR;
    // }
    // if (spi_write_blocking(in_spi_hw, TX_buffer, length) != length){
    //     gpio_put(in_cs, 1);
    //     return pico_canlib::status::TX_PAYLOAD_ERROR;
    // }

    XL2515::write_buffer message;
    message.can_id = can_id;
    message.payload = TX_buffer;
    message.id_TXBuffer_instr |= TX_ID;
    message.payload_TXBuffer_instr |= TX_ID;

    if (spi_write_blocking(in_spi_hw, (uint8_t *) &message, XL2515::write_buffer_len) != XL2515::write_buffer_len){
        gpio_put(in_cs, 1);
        return pico_canlib::status::TX_PAYLOAD_ERROR;
    }

    //Request to send CAN message
    if (TX_ID == 0){
        requestTS(1);
    }
    else {
        requestTS(TX_ID);
    }

    // Pull CS high to deselect the transceiver
    gpio_put(in_cs, 1);
    return pico_canlib::status::SUCCESS;
}

pico_canlib::status pico_canlib::receiveCAN(uint8_t RX_ID, uint8_t * buffer, uint8_t idSize = 4, uint8_t bufferSize = 8)
{
    // Pull CS low to select the device
    gpio_put(in_cs, 0);
    // Read ID over SPI
    uint8_t instruction = (uint8_t) XL2515::SPI_INSTR_XL::READ_RX_BUFF | RX_ID;
    if (spi_read_blocking(in_spi_hw, instruction, &buffer[0], idSize) != idSize){
        gpio_put(in_cs, 1);
        return pico_canlib::status::RX_ID_ERROR;
    };

    // Read DATA over SPI
    instruction = (uint8_t) XL2515::SPI_INSTR_XL::READ_RX_BUFF | (RX_ID + 2);
    if (spi_read_blocking(in_spi_hw, instruction, &buffer[idSize], bufferSize) != bufferSize){
        gpio_put(in_cs, 1);
        return pico_canlib::status::RX_PAYLOAD_ERROR;
    };

    // Pull CS high to deselect the device
    gpio_put(in_cs, 1);

    return pico_canlib::status::SUCCESS;
}