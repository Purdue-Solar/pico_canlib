#include "pico_canlib.hpp"

/// @brief Initialized spi ports and reset XL2515 Configuration
void pico_canlib::init(void)
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
    reset();
}

/// @brief Reset XL2515 Configuration
void pico_canlib::reset(){
    gpio_put(in_cs, 0);
    uint8_t data = XL2515::SPI_INSTR_XL::RESET;
    spi_write_blocking(in_spi_hw, &data, 8);
    gpio_put(in_cs, 1);
}

/// @brief Deivice Request To Send Signal
/// @param buffer Determine which TX buffer to use
void pico_canlib::requestTS(uint8_t buffer){
    uint8_t data = XL2515::SPI_INSTR_XL::READ | buffer;
    spi_write_blocking(in_spi_hw, &data, 8);
}

bool pico_canlib::checkStatus(uint8_t RX_ID){
    uint8_t data = XL2515::SPI_INSTR_XL::READ_STATUS;
    uint8_t temp;
    if (spi_read_blocking(in_spi_hw, data, &temp, 1) != 1){
        return false;
    };

    if ((temp & 0x1)){ // Check RX0IF || RX1IF pin
        return false;
    }
    
    return true;
}

/// @brief Send SPI request to send CAN messages
/// @param TX_ID TX Buffer select
/// @param id    29 bits extended ids as bytes array
/// @param TX_buffer  Payload bytes array
/// @param length Length in bytes of payload
/// @return True if SPI request was successful sent
bool pico_canlib::sendCAN(uint8_t TX_ID, uint8_t * id, uint8_t idSize, uint8_t* TX_buffer, size_t length)
{
    // Pull CS low to select the transceiver
    gpio_put(in_cs, 0);

    //Request to send CAN message
    requestTS(1 << TX_ID);

    // Send id over SPI
    XL2515::dir_load_tx_buffer buffer;
    buffer.instruction = XL2515::SPI_INSTR_XL::LOAD_TX_BUFF | TX_ID;
    buffer.payload = id;
    if (spi_write_blocking(in_spi_hw, (uint8_t *) &buffer, idSize) != idSize){
        return false;
    }

    // Send data over SPI
    buffer.instruction = XL2515::SPI_INSTR_XL::LOAD_TX_BUFF | (TX_ID + 3); // TXx_ID + 3 = TXx_DAT
    buffer.payload = TX_buffer;  
    if (spi_write_blocking(in_spi_hw, (uint8_t *) &buffer, length) != length){
        return false;
    }

    // Pull CS high to deselect the transceiver
    gpio_put(in_cs, 1);
    return true;
}

/// @brief Send SPI request to recieve CAN messages
/// @param RX_ID RX Buffer select
/// @param buffer Bytes array to save payload
/// @param idSize Length in bytes of id
/// @param bufferSize Length in bytes of payload
/// @return True if SPI request was successful sent
bool pico_canlib::receiveCAN(uint8_t RX_ID, uint8_t * buffer, uint8_t idSize = 4, uint8_t bufferSize = 8)
{
    // Pull CS low to select the device
    gpio_put(in_cs, 0);
    // Read ID over SPI
    XL2515::dir_read_rx_buffer read_request;
    read_request.instruction = XL2515::SPI_INSTR_XL::READ_RX_BUFF | RX_ID;
    if (spi_read_blocking(in_spi_hw, read_request.instruction, &buffer[0], idSize) != idSize){
        return false;
    };

    // Read DATA over SPI
    read_request.instruction = XL2515::SPI_INSTR_XL::READ_RX_BUFF | (RX_ID + 2);
    if (spi_read_blocking(in_spi_hw, read_request.instruction, &buffer[idSize], bufferSize) != bufferSize){
        return false;
    };

    // Pull CS high to deselect the device
    gpio_put(in_cs, 1);

    return true;
}
