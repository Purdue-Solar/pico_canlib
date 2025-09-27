#include "RP2040_XL2515_Driver.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_INT   8
#define PIN_CS    9
#define PIN_SCK  10
#define PIN_MOSI 11
#define PIN_MISO 12

XL2515_Driver::XL2515_Driver()
{
    // Constructor
}

XL2515_Driver::~XL2515_Driver()
{
    // Destructor
}

void XL2515_Driver::init(void)
{
    // Initialize SPI port at 1 MHz
    spi_init(SPI_PORT, 1000 * 1000);

    // Set the GPIO functions for the SPI pins
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS, GPIO_FUNC_SIO); // CS pin as SIO for manual control
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    // Set CS pin high (inactive)
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
}

void XL2515_Driver::sendData(const uint8_t* data, size_t length)
{
    // Pull CS low to select the device
    gpio_put(PIN_CS, 0);

    // Send data over SPI
    spi_write_blocking(SPI_PORT, data, length);

    // Pull CS high to deselect the device
    gpio_put(PIN_CS, 1);
}

size_t XL2515_Driver::receiveData(uint8_t* buffer, size_t bufferSize)
{
    // Pull CS low to select the device
    gpio_put(PIN_CS, 0);

    // Read data over SPI
    size_t bytesRead = spi_read_blocking(SPI_PORT, 0x00, buffer, bufferSize);

    // Pull CS high to deselect the device
    gpio_put(PIN_CS, 1);

    return bytesRead;
}
