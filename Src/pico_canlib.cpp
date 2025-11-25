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
    uint8_t data = XL2515::SPI_INSTR_XL::RESET;
    if (spi_write_blocking(in_spi_hw, &data, 8) != 8){
        return pico_canlib::status::RESET_ERROR;
    }
    else {
        return pico_canlib::status::SUCCESS;
    }
    gpio_put(in_cs, 1);
}

pico_canlib::status pico_canlib::requestTS(uint8_t buffer){
    uint8_t data = XL2515::SPI_INSTR_XL::READ | buffer;
    if (spi_write_blocking(in_spi_hw, &data, 8) != 8){
        return pico_canlib::status::REQUESTTS_ERROR;
    }
    else{
        return pico_canlib::status::SUCCESS;
    }
}

pico_canlib::status pico_canlib::checkRXStatus(uint8_t buffer){
    uint8_t data = XL2515::SPI_INSTR_XL::READ_STATUS;
    uint8_t temp;
    if (spi_read_blocking(in_spi_hw, data, &temp, 1) != 1){
        return pico_canlib::status::RX_STATUS_ERROR;
    };

    if ((temp & buffer)){ // Check RX0IF || RX1IF pin assuming 0x1 is still in progress and 0x0 is not
        return pico_canlib::status::RX_STATUS_STALL;
    }
    
    return pico_canlib::status::RX_STATUS_DONE;
}

pico_canlib::status pico_canlib::transmitCAN(uint8_t TX_ID, uint8_t * can_id, uint8_t idSize, uint8_t* TX_buffer, size_t length)
{
    // Pull CS low to select the transceiver
    gpio_put(in_cs, 0);

    //Request to send CAN message
    requestTS(1 << TX_ID);

    // Send id over SPI
    // XL2515::dir_load_tx_buffer buffer;
    uint8_t instruction = XL2515::SPI_INSTR_XL::LOAD_TX_BUFF | TX_ID;
    if (spi_write_blocking(in_spi_hw, &instruction, 1) != 1){
        gpio_put(in_cs, 1);
        return pico_canlib::status::TX_ID_COMMAND_ERROR;
    }
    
    if (spi_write_blocking(in_spi_hw, can_id, idSize) != idSize){
        gpio_put(in_cs, 1);
        return pico_canlib::status::TX_ID_ERROR;
    }

    // Send data over SPI
    instruction = XL2515::SPI_INSTR_XL::LOAD_TX_BUFF | (TX_ID + 3); // TXx_ID + 3 = TXx_DAT
    if (spi_write_blocking(in_spi_hw, &instruction, 1) != 1){
        gpio_put(in_cs, 1);
        return pico_canlib::status::TX_PAYLOAD_COMMAND_ERROR;
    }
    if (spi_write_blocking(in_spi_hw, TX_buffer, length) != length){
        gpio_put(in_cs, 1);
        return pico_canlib::status::TX_PAYLOAD_ERROR;
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
    uint8_t instruction = XL2515::SPI_INSTR_XL::READ_RX_BUFF | RX_ID;
    if (spi_read_blocking(in_spi_hw, instruction, &buffer[0], idSize) != idSize){
        gpio_put(in_cs, 1);
        return pico_canlib::status::RX_ID_ERROR;
    };

    // Read DATA over SPI
    instruction = XL2515::SPI_INSTR_XL::READ_RX_BUFF | (RX_ID + 2);
    if (spi_read_blocking(in_spi_hw, instruction, &buffer[idSize], bufferSize) != bufferSize){
        gpio_put(in_cs, 1);
        return pico_canlib::status::RX_PAYLOAD_ERROR;
    };

    // Pull CS high to deselect the device
    gpio_put(in_cs, 1);

    return pico_canlib::status::SUCCESS;
}