#include "pico_canlib.hpp"

// Marks spi_depth nonzero for the duration of a public API call so the keypad
// ISR never borrows the shared SCK pad (matrix column 2, GPIO 10) while a
// transaction can be mid-burst. Depth-counted because public methods nest
// (e.g. transmitCAN -> setByte). Single-core: the ISR only reads the flag.
// INVARIANT: every public method that can touch CS/SCK must open with one.
namespace {
struct SpiBusyGuard {
  volatile uint8_t &depth;
  explicit SpiBusyGuard(volatile uint8_t &d) : depth(d) { depth = depth + 1; }
  ~SpiBusyGuard() { depth = depth - 1; }
};
} // namespace

pico_canlib::status pico_canlib::init(void) {
  SpiBusyGuard busy(spi_depth);
  // Initialize SPI port at 1 MHz
  spi_init(in_spi_hw, XL2515::XL2515_BAUDRATE);

  // Set the GPIO functions for the SPI pins
  gpio_set_function(in_miso, GPIO_FUNC_SPI);
  gpio_set_function(in_mosi, GPIO_FUNC_SPI);
  gpio_set_function(in_cs, GPIO_FUNC_SIO); // CS pin as SIO for manual control
  gpio_set_function(in_sck, GPIO_FUNC_SPI);

  // Set CS pin high (inactive)
  gpio_init(in_cs);
  gpio_set_dir(in_cs, GPIO_OUT);
  gpio_put(in_cs, 1);

  // Reset XL2515 Configuration
  status status = reset();
  fprintf(stdout, "Status Bytes = %d\n", status);
  if (status != status::SUCCESS) {
    return status;
  }

  // Oscillator start-up time: the MCP2515 holds its registers unreliable for
  // 128 Tosc after the RESET command (~16 us at 8 MHz, ~8 us at 16 MHz). Wait
  // before the first CNF write. This previously worked only by accident, riding
  // on the delay of the fprintf below — which vanishes when stdio USB is absent
  // (in-car) or the debug prints are removed. RESET also leaves the device in
  // Configuration mode, which is required for the CNF writes that follow.
  sleep_us(50);

  uint8_t mode;

  // RX acceptance config (finding M-10). POR leaves RXB0CTRL.BUKT=0 (no
  // RXB0->RXB1 rollover, effective RX depth is ONE frame) and the filter/mask
  // registers undefined, so acceptance is formally indeterminate until this
  // runs. All writes below must happen in Configuration mode, before the
  // Normal-mode transition further down.
  //
  // Zero every mask register: with RXM=00 ("use filters/masks", set below) a
  // zero mask means "don't care" on every ID bit, so all valid frames are
  // accepted without resorting to RXM=11, which the datasheet (S4.2.2) flags
  // as debug-only. Filters are zeroed too even though a zero mask makes their
  // value irrelevant -- POR filter contents are undefined, and this avoids
  // the standard-frame data-byte filtering gotcha the datasheet calls out.
  //
  // Three separate auto-increment writes because the filter/mask blocks are
  // split by the CANSTAT/CANCTRL mirror registers at 0x0C-0x0F.
  {
    static const uint8_t zeros12[12] = {0}; // RXF0-RXF2 / RXF3-RXF5
    static const uint8_t zeros8[8] = {0};   // RXM0 / RXM1
    if (filtersAndMasks(zeros12, sizeof(zeros12),
                         XL2515::IN_ADDR::RXF0SIDH) != status::SUCCESS) {
      return status::INIT_ERROR;
    }
    if (filtersAndMasks(zeros12, sizeof(zeros12),
                         XL2515::IN_ADDR::RXF3SIDH) != status::SUCCESS) {
      return status::INIT_ERROR;
    }
    if (filtersAndMasks(zeros8, sizeof(zeros8),
                         XL2515::IN_ADDR::RXM0SIDH) != status::SUCCESS) {
      return status::INIT_ERROR;
    }
  }

  fprintf(stdout, "RXB0CTRL SET. Error Status: %d\n",
          setByte(XL2515::RXB0CTRL_RXALL_BUKT, XL2515::IN_ADDR::RXB0CTRL));
  getByte(&mode, XL2515::IN_ADDR::RXB0CTRL);
  fprintf(stdout, "RXB0CTRL Bytes = %d\n", mode);

  // Compare only the writable bits, RXM<1:0> and BUKT (mask 0x6C): bit 1
  // (BUKT1) is a read-only mirror of BUKT (datasheet Register 4-1), so with
  // BUKT set the register reads back 0x06 and a full-byte compare against
  // 0x04 fails on every boot, aborting init() with the chip still in
  // Configuration mode. RXRTR/FILHIT0 are read-only status bits, ignored.
  if ((mode & 0x6C) != XL2515::RXB0CTRL_RXALL_BUKT) {
    return status::INIT_ERROR;
  }

  fprintf(stdout, "RXB1CTRL SET. Error Status: %d\n",
          setByte(XL2515::RXB1CTRL_RXALL, XL2515::IN_ADDR::RXB1CTRL));
  getByte(&mode, XL2515::IN_ADDR::RXB1CTRL);
  fprintf(stdout, "RXB1CTRL Bytes = %d\n", mode);

  // Same masking discipline: only RXM<1:0> (mask 0x60) is writable here;
  // RXRTR and FILHIT<2:0> are read-only status bits.
  if ((mode & 0x60) != XL2515::RXB1CTRL_RXALL) {
    return status::INIT_ERROR;
  }

  // Set Control Bits
  fprintf(stdout, "CNF1 SET. Error Status: %d\n",
          setByte(XL2515::NORMAL_CNF1, XL2515::IN_ADDR::CNF1));
  getByte(&mode, XL2515::IN_ADDR::CNF1);
  fprintf(stdout, "CANINTE Bytes = %d\n", mode);

  if (mode != XL2515::NORMAL_CNF1) {
    return status::INIT_ERROR;
  }

  fprintf(stdout, "CNF1 SET. Error Status: %d\n",
          setByte(XL2515::NORMAL_CNF2, XL2515::IN_ADDR::CNF2));
  getByte(&mode, XL2515::IN_ADDR::CNF2);
  fprintf(stdout, "CANINTE Bytes = %d\n", mode);

  if (mode != XL2515::NORMAL_CNF2) {
    return status::INIT_ERROR;
  }

  fprintf(stdout, "CNF1 SET. Error Status: %d\n",
          setByte(XL2515::NORMAL_CNF3, XL2515::IN_ADDR::CNF3));
  getByte(&mode, XL2515::IN_ADDR::CNF3);
  fprintf(stdout, "CANINTE Bytes = %d\n", mode);

  if (mode != XL2515::NORMAL_CNF3) {
    return status::INIT_ERROR;
  }

  fprintf(stdout, "CANINTE SET. Error Status: %d\n",
          setByte(XL2515::INTE_EN, XL2515::IN_ADDR::CANINTE));
  getByte(&mode, XL2515::IN_ADDR::CANINTE);
  fprintf(stdout, "CANINTE Bytes = %d\n", mode);

  if (mode != XL2515::INTE_EN) {
    return status::INIT_ERROR;
  }

  fprintf(stdout, "CANCTRL SET. Error Status: %d\n",
          setByte(XL2515::NORMAL_MODE, XL2515::IN_ADDR::CANCTRL));
  // Confirm the chip actually entered Normal mode by reading CANSTAT.OPMOD
  // (bits 7:5), NOT by reading back the CANCTRL request register. The old check
  // read CANCTRL and compared it to LOOPBACK_MODE (0x40); since we just wrote
  // NORMAL_MODE (0x00) it could never match, so a failed mode transition was
  // reported as SUCCESS. Normal mode = OPMOD 0b000. The transition can lag the
  // request (it waits for the bus to go idle), so poll with a bounded timeout.
  mode = 0xFF;
  for (int attempt = 0; attempt < 10; attempt++) {
    if (getByte(&mode, XL2515::IN_ADDR::CANSTAT) != status::SUCCESS) {
      mode = 0xFF; // a failed read must not masquerade as OPMOD == Normal
      sleep_us(100);
      continue;
    }
    if ((mode & 0xE0) == 0x00) {
      break;
    }
    sleep_us(100);
  }
  fprintf(stdout, "CANSTAT Bytes = %d\n", mode);

  if ((mode & 0xE0) != 0x00) {
    return status::INIT_ERROR;
  }

  return status::SUCCESS;
}

pico_canlib::status pico_canlib::filtersAndMasks(const uint8_t *data,
                                                 uint8_t length,
                                                 XL2515::IN_ADDR addr) {
  // Rewrite of the old VLA-based version, which hardcoded a 14-byte SPI write
  // regardless of `length`: for length < 14 that read past the end of the
  // stack buffer and clocked out garbage into whatever registers followed
  // (potentially CNF/CANINTE), and for length != 14 the return-value compare
  // against `length` was always wrong. This version writes exactly `length`
  // bytes from the caller-owned buffer -- no VLA, no size mismatch possible.
  SpiBusyGuard busy(spi_depth);
  uint8_t header[2] = {(uint8_t)XL2515::SPI_INSTR_XL::WRITE, (uint8_t)addr};

  gpio_put(in_cs, 0);
  bool ok = spi_write_blocking(in_spi_hw, header, 2) == 2 &&
            spi_write_blocking(in_spi_hw, data, length) == length;
  gpio_put(in_cs, 1);

  return ok ? status::SUCCESS : status::WRITE_ERROR;
}

pico_canlib::status pico_canlib::setByte(uint8_t bytes, XL2515::IN_ADDR addr) {
  SpiBusyGuard busy(spi_depth);
  uint8_t message[4];
  message[0] = (uint8_t)XL2515::SPI_INSTR_XL::WRITE;
  message[1] = (uint8_t)addr;
  // message[2] = 0xFF;
  message[2] = bytes;

  gpio_put(in_cs, 0);

  if (spi_write_blocking(in_spi_hw, message, 3) != 3) {
    gpio_put(in_cs, 1);
    return status::WRITE_ERROR;
  }

  gpio_put(in_cs, 1);

  return status::SUCCESS;
}

pico_canlib::status pico_canlib::getByte(uint8_t *bytes, XL2515::IN_ADDR addr) {
  SpiBusyGuard busy(spi_depth);
  uint8_t data[3];
  data[0] = (uint8_t)XL2515::SPI_INSTR_XL::READ;
  data[1] = (uint8_t)addr;

  gpio_put(in_cs, 0);

  spi_write_blocking(in_spi_hw, data, 2);
  if (spi_read_blocking(in_spi_hw, 0, bytes, 1) != 1) {
    gpio_put(in_cs, 1);
    return status::GET_CONTROL_BITS_ERROR;
  }

  gpio_put(in_cs, 1);

  return status::SUCCESS;
}

pico_canlib::status pico_canlib::reset() {
  gpio_put(in_cs, 0);
  uint8_t data = (uint8_t)XL2515::SPI_INSTR_XL::RESET;
  if (spi_write_blocking(in_spi_hw, &data, 1) != 1) {
    gpio_put(in_cs, 1);
    return pico_canlib::status::RESET_ERROR;
  }
  gpio_put(in_cs, 1);
  return pico_canlib::status::SUCCESS;
}

pico_canlib::status pico_canlib::requestTS(uint8_t buffer) {
  uint8_t data = (uint8_t)XL2515::SPI_INSTR_XL::REQTS | (1 << buffer);
  gpio_put(in_cs, 0);
  if (spi_write_blocking(in_spi_hw, &data, 1) != 1) {
    gpio_put(in_cs, 1);
    return pico_canlib::status::REQUESTTS_ERROR;
  }
  gpio_put(in_cs, 1);
  return pico_canlib::status::SUCCESS;
}

pico_canlib::status pico_canlib::checkStatus(uint8_t *status) {
  SpiBusyGuard busy(spi_depth);
  uint8_t instr = (uint8_t)XL2515::SPI_INSTR_XL::READ_STATUS;
  gpio_put(in_cs, 0);
  spi_write_blocking(in_spi_hw, &instr, 1);
  if (spi_read_blocking(in_spi_hw, 0, status, 1) != 1) {
    gpio_put(in_cs, 1);
    return pico_canlib::status::STATUS_ERROR;
  }
  gpio_put(in_cs, 1);
  return pico_canlib::status::SUCCESS;
}

// bit modify helper used to set/clear individual bits in a register
pico_canlib::status pico_canlib::modifiedBit(uint8_t bytes, uint8_t address,
                                             uint8_t masked) {
  XL2515::bit_modify msg;
  msg.addr = address;
  msg.mask = masked;
  msg.payload = bytes;

  gpio_put(in_cs, 0);
  if (spi_write_blocking(in_spi_hw, (uint8_t *)&msg, 4) != 4) {
    gpio_put(in_cs, 1);
    return pico_canlib::status::MODIFIED_ERROR;
  }
  gpio_put(in_cs, 1);
  return pico_canlib::status::SUCCESS;
}

pico_canlib::status pico_canlib::transmitCAN(XL2515::TX_BUFFER_SEL TX_SEL,
                                             uint32_t can_id, bool isExtended,
                                             uint8_t *data_buffer,
                                             uint8_t data_length,
                                             XL2515::PRIORITY priority) {
  SpiBusyGuard busy(spi_depth);
  uint8_t TX_ID = (uint8_t)TX_SEL;
  // Set Data Length
  setByte(data_length,
          (XL2515::IN_ADDR)((uint8_t)XL2515::IN_ADDR::TXBxDLC +
                            (TX_ID
                             << 4))); // Weird ah implementation but we made do

  // Set ID
  if (isExtended) {
    setByte(
        (can_id >> 16 | 0x4),
        (XL2515::IN_ADDR)((uint8_t)XL2515::IN_ADDR::TXBxSIDL + (TX_ID << 4)));
    setByte(
        (can_id >> 8),
        (XL2515::IN_ADDR)((uint8_t)XL2515::IN_ADDR::TXBxEID8 + (TX_ID << 4)));
    setByte(can_id, (XL2515::IN_ADDR)((uint8_t)XL2515::IN_ADDR::TXBxEID0 +
                                      (TX_ID << 4)));
    setByte(0, (XL2515::IN_ADDR)((uint8_t)XL2515::IN_ADDR::TXBxSIDL +
                                 (TX_ID << 4)));
  } else {
    setByte(can_id >> 3, (XL2515::IN_ADDR)((uint8_t)XL2515::IN_ADDR::TXBxSIDH +
                                           (TX_ID << 4)));
    setByte(
        (can_id << 5),
        (XL2515::IN_ADDR)((uint8_t)XL2515::IN_ADDR::TXBxSIDL + (TX_ID << 4)));
    setByte(0, (XL2515::IN_ADDR)((uint8_t)XL2515::IN_ADDR::TXBxEID0 +
                                 (TX_ID << 4)));
    setByte(0, (XL2515::IN_ADDR)((uint8_t)XL2515::IN_ADDR::TXBxEID8 +
                                 (TX_ID << 4)));
  }

  // Set Priority
  XL2515::bit_modify masked_message;
  masked_message.addr = (uint8_t)XL2515::IN_ADDR::TXBxCTRL + (TX_ID << 4);
  masked_message.mask = 0x03;
  masked_message.payload = (uint8_t)priority;

  gpio_put(in_cs, 0);
  if (spi_write_blocking(in_spi_hw, (uint8_t *)&masked_message, 4) != 4) {
    gpio_put(in_cs, 1);
    return pico_canlib::status::TX_PAYLOAD_COMMAND_ERROR;
  }
  gpio_put(in_cs, 1);

  // Set Data
  // XL2515::load_tx_buffer message;
  uint8_t instr = 0x41 + (TX_ID * 2);

  gpio_put(in_cs, 0);
  spi_write_blocking(in_spi_hw, &instr, 1);
  spi_write_blocking(in_spi_hw, data_buffer, data_length);
  // if (spi_write_blocking(in_spi_hw, (uint8_t *)&message, data_length + 1) !=
  // (data_length + 1))
  // {
  //     gpio_put(in_cs, 1);
  //     return pico_canlib::status::TX_PAYLOAD_COMMAND_ERROR;
  // }

  gpio_put(in_cs, 1);

  // Request to send CAN message
  return requestTS(TX_ID);
}

// flow:
// 1. poll READ_STATUS (0xb0) (part of main)
// 2. check if bit 0 and 1 of received byte
// 3. if bit is high, send READ RX BUFFER command
// 4. clear CANINTF via bitmodify
pico_canlib::status
pico_canlib::receiveCAN(/*uint8_t rxstat, uint8_t RX_ID,*/ uint8_t *buffer,
                        uint8_t idSize = 4, uint8_t bufferSize = 8) {
  SpiBusyGuard busy(spi_depth);
  uint8_t st = 0;
  status errorCode = checkStatus(&st);
  if (errorCode != status::SUCCESS) {
    return errorCode;
  }

  // READ STATUS byte layout (datasheet Figure 12-8): bit0 = RX0IF, bit1 = RX1IF,
  // bits 2-7 = TX status (TXREQ / TXnIF). TXnIF latches on every completed
  // transmit and is never cleared, so comparing the whole byte to 0x01/0x02/0x03
  // wrongly reports "no message" as soon as any frame has been sent. Mask to the
  // RX flags only.
  uint8_t rx = st & 0x03;
  if (rx == 0) {
    return status::NO_NEW_MESSAGE;
  }
  // Service RXB0 first when both are pending (RXB0 is higher priority); the
  // caller's drain loop collects RXB1 on the next call.
  uint8_t RX_ID = (rx & 0x01) ? 0x00 : 0x01;

  // READ RX BUFFER command + read
  uint8_t instr = (uint8_t)XL2515::SPI_INSTR_XL::READ_RX_BUFF | (RX_ID << 2);

  gpio_put(in_cs, 0);
  if (spi_write_blocking(in_spi_hw, &instr, 1) != 1) {
    gpio_put(in_cs, 1);
    return status::RX_ID_ERROR;
  }

  // reads ID, data length, buffer
  uint16_t totalBytes = (uint16_t)idSize + (uint16_t)bufferSize + 1;
  if (spi_read_blocking(in_spi_hw, 0, buffer, totalBytes) != totalBytes) {
    gpio_put(in_cs, 1);
    return status::RX_PAYLOAD_ERROR;
  }
  // READ RX BUFFER auto-clears this buffer's RXnIF when CS rises here (datasheet
  // section 12.4), so no manual CANINTF clear is needed. The old explicit clear
  // also raced a frame arriving during this read and could wipe its fresh flag.
  gpio_put(in_cs, 1);

  // Standard 11-bit ID from RXBnSIDH:RXBnSIDL, written back LITTLE-endian to
  // match the caller's memcpy(&id, buffer, 4) on this little-endian MCU.
  uint32_t id = (buffer[0] << 3) | (buffer[1] >> 5);
  buffer[0] = (id) & 0xff;
  buffer[1] = (id >> 8) & 0xff;
  buffer[2] = (id >> 16) & 0xff;
  buffer[3] = (id >> 24) & 0xff;

  // DLC: only the low nibble is the data length; the RTR/reserved bits share the
  // register byte and must be masked off before the caller uses it as a length.
  buffer[4] &= 0x0F;

  return status::SUCCESS;
}
