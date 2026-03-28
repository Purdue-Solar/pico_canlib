# Pico CAN Library

This is a CAN library for the RP2350b.

## Function Calls

- Initialize a `pico_canlib` object
- Call `init()`
- To transmit: `transmitCAN(tx buffer select, CAN id, false, array of data to transmit, length of data array, priority)`
- To receive: `receiveCAN(data buffer, id size (4), data buffer size (8))`
