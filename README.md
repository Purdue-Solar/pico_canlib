
# CANLib

A common C++ interface for **classical CAN 2.0** on three microcontroller targets:

- **Adafruit ESP32 Feather V2 HUZZAH** — ESP32 TWAI controller
- **Waveshare RP2350-CAN** — XL2515 CAN controller connected over SPI
- **Teensy 4.0** — FlexCAN controller with the `FlexCAN_T4` library

Applications use the same `canlib` API, while the build selects one platform-specific implementation. The current implementations use **standard (11-bit) CAN identifiers**, not 29-bit extended identifiers or CAN FD. Payloads are 0–8 bytes.

> **Status:** This README describes the source files and PlatformIO setup discussed for this library. The platform implementations and interrupt integration are not yet hardware-validated. See [Known limitations](#known-limitations) before deployment.

## 1. Repository layout

```text
CANLib/
├── library.json
├── select_platform.py
├── README.md
└── src/
    ├── CANDevice.h
    ├── canlib.h
    ├── canlib.cpp
    ├── esp32/
    │   ├── esp_canlib.h
    │   └── esp_canlib.cpp
    ├── rp2350/
    │   ├── pico_canlib.h
    │   ├── pico_canlib.cpp
    │   └── MCP2515.h
    └── teensy/
        ├── teensy_canlib.h
        └── teensy_canlib.cpp
```

`canlib.h` contains the abstract base class and `status` enum. `CANDevice.h` exposes a platform-independent name (`CANDevice`). Only one platform implementation should be built for each firmware image.

### `src/CANDevice.h`

```cpp
#pragma once

#include "canlib.h"

#if defined(CAN_TARGET_ESP32)
#include "esp32/esp_canlib.h"
using CANDevice = esp_canlib;
#elif defined(CAN_TARGET_RP2350)
#include "rp2350/pico_canlib.h"
using CANDevice = pico_canlib;
#elif defined(CAN_TARGET_TEENSY)
#include "teensy/teensy_canlib.h"
using CANDevice = teensy_canlib;
#else
#error "Select a CAN_TARGET_* build flag"
#endif
```

## 2. Installation with PlatformIO

The example setup assumes the **Arduino framework** on each board. The RP2350 configuration uses the Arduino-Pico core; a standalone Pico SDK application requires a separate CMake integration.

### Install from a local copy

Place the `CANLib` directory inside the application project's `lib/` directory:

```text
MyProject/
├── platformio.ini
├── src/
│   └── main.cpp
└── lib/
    └── CANLib/
        ├── library.json
        ├── select_platform.py
        └── src/
            └── ...
```

Alternatively, after publishing the library to GitHub, reference the repository from the application's `lib_deps`:

```ini
lib_deps =
    https://github.com/Purdue-Solar/pico_canlib#general
```

Replace the example URL with the actual repository URL.

### `library.json`

Create this file in the library root:

```json
{
    "name": "CANLib",
    "version": "1.0.0",
    "description": "Classical CAN interface for ESP32, RP2350, and Teensy",
    "frameworks": ["arduino"],
    "platforms": "*",
    "headers": ["CANDevice.h", "canlib.h"],
    "build": {
        "srcDir": "src",
        "includeDir": "src",
        "extraScript": "select_platform.py"
    }
}
```

### `select_platform.py`

This library build script limits compilation to `canlib.cpp` and the selected backend:

```python
Import("env")

targets = {
    "CAN_TARGET_ESP32": "esp32",
    "CAN_TARGET_RP2350": "rp2350",
    "CAN_TARGET_TEENSY": "teensy"
}

def define_name(item):
    return item[0] if isinstance(item, tuple) else item

defines = {define_name(item) for item in env.get("CPPDEFINES", [])}
selected = [folder for macro, folder in targets.items() if macro in defines]

if len(selected) != 1:
    raise RuntimeError("Select exactly one CAN_TARGET_* build flag")

env.Replace(SRC_FILTER=["+<canlib.cpp>", f"+<{selected[0]}/*.cpp>"])
print(f"CANLib: Building {selected[0]} backend")
```

**Build-script caution:** Test the library-specific source filtering with your installed PlatformIO version. If PlatformIO's library dependency finder doesn't discover the library through `CANDevice.h`, add `lib_deps` explicitly or include the library as a local `lib/CANLib` directory.

## 3. Select the target board

In your application's `platformio.ini`:

```ini
[platformio]
default_envs = esp32

[env]
framework = arduino
monitor_speed = 115200

[env:esp32]
platform = espressif32
board = adafruit_feather_esp32_v2
build_flags = -DCAN_TARGET_ESP32

[env:rp2350]
platform = https://github.com/maxgerhardt/platform-raspberrypi.git
board = rpipico2
board_build.core = earlephilhower
build_flags = -DCAN_TARGET_RP2350

[env:teensy40]
platform = teensy
board = teensy40
build_flags = -DCAN_TARGET_TEENSY
lib_deps =
    https://github.com/tonton81/FlexCAN_T4.git
```

The `rpipico2` board entry is a generic RP2350 configuration. Check board-specific flash and upload settings for your Waveshare board. ESP32 builds require a framework version providing the legacy `driver/twai.h` API.

Change `default_envs` to select which environment `pio run` builds:

```ini
[platformio]
default_envs = teensy40
```

Or select a target without editing the file:

```bash
pio run -e esp32
pio run -e rp2350
pio run -e teensy40
```

Upload and monitor, for example:

```bash
pio run -e teensy40 -t upload
pio device monitor -b 115200
```

In VS Code, expand **PlatformIO → Project Tasks → env:<target> → General** and choose **Build** or **Upload**.

## 4. Hardware connections

All three boards require a CAN physical-layer transceiver. The ESP32 and Teensy have CAN controllers but **do not** connect directly to CAN_H/CAN_L.

| Target | Controller connection | Notes |
|---|---|---|
| ESP32 Feather V2 | GPIO27 → transceiver TXD; GPIO33 ← transceiver RXD | Use a 3.3 V logic-compatible CAN transceiver |
| RP2350-CAN | Onboard XL2515 via SPI; INT → GPIO8, CS → GPIO9, SCK → GPIO10, MOSI → GPIO11, MISO → GPIO12 | Built-in CAN transceiver on Waveshare board |
| Teensy 4.0 | CAN1 TX pin 22 → transceiver TXD; CAN1 RX pin 23 ← transceiver RXD | Use a 3.3 V logic-compatible CAN transceiver |

Connect grounds appropriately and terminate the CAN bus with **120 Ω at each physical end**, not at every node. Set **the same CAN bitrate** on all nodes.

## 5. Basic application

Because `CANDevice` is selected at build time, the application doesn't need to include a particular platform header.

```cpp
#include <Arduino.h>
#include <CANDevice.h>

CANDevice can(500000);

void setup()
{
    Serial.begin(115200);

    status result = can.init();
    if (result != status::SUCCESS)
    {
        Serial.printf("CAN init failed: %u\n", static_cast<unsigned>(result));
        return;
    }

    result = can.init_irq();
    if (result != status::SUCCESS)
    {
        Serial.printf("CAN IRQ init failed: %u\n", static_cast<unsigned>(result));
        return;
    }
}

void loop()
{
    uint8_t buffer[13] = {};

    while (true)
    {
        status result = can.receiveCAN(buffer, 4, 8);
        if (result == status::NO_NEW_MESSAGE)
        {
            break;
        }
        if (result != status::SUCCESS)
        {
            Serial.printf("CAN RX error: %u\n", static_cast<unsigned>(result));
            break;
        }

        uint32_t id = static_cast<uint32_t>(buffer[0]) |
                      (static_cast<uint32_t>(buffer[1]) << 8) |
                      (static_cast<uint32_t>(buffer[2]) << 16) |
                      (static_cast<uint32_t>(buffer[3]) << 24);
        uint8_t length = buffer[4];
        uint8_t *payload = buffer + 5;

        Serial.printf("RX ID=0x%03lX length=%u\n", static_cast<unsigned long>(id), length);
        for (uint8_t i = 0; i < length; i++)
        {
            Serial.printf("%02X ", payload[i]);
        }
        Serial.println();
    }
}
```

### Send a frame

```cpp
uint8_t payload[3] = {0x10, 0x20, 0x30};
status result = can.transmitCAN(0x123, payload, sizeof(payload));

if (result != status::SUCCESS)
{
    // Handle error or retry later (e.g., TX_STATUS_STALL).
}
```

`transmitCAN()` returning `SUCCESS` means the implementation accepted the transmission request; it does **not** necessarily mean another node acknowledged the frame.

### Receive-buffer layout

`receiveCAN(buffer, 4, 8)` expects **at least 13 writable bytes** and returns:

| Byte(s) | Meaning |
|---|---|
| `0..3` | Standard CAN ID as a 32-bit little-endian value |
| `4` | Number of payload bytes (0–8) |
| `5..12` | Data payload; remaining bytes are zero-filled |

The function is nonblocking. It returns `status::NO_NEW_MESSAGE` when no message is ready. Check for `status::SUCCESS` before reading the output buffer.

## 6. Interrupt handling

Call the two initialization methods in order:

```cpp
can.init();
can.init_irq();
```

The interface exposes `can_isr()`, but **applications should not install an additional interrupt handler that calls it without understanding the backend**:

- **ESP32:** The TWAI driver's hardware ISR fills its own receive queue. An optional FreeRTOS alert task calls `can_isr()` to mark pending input. `receiveCAN()` drains the TWAI queue.
- **RP2350:** The XL2515's active-low interrupt reaches GPIO8. The GPIO callback calls `can_isr()`, which only sets a pending flag. Reading the controller via SPI happens in `receiveCAN()` outside interrupt context.
- **Teensy:** FlexCAN_T4 receives messages through its mailbox interrupt machinery. `can_isr()` calls `can.events()` in normal execution context to dispatch queued receive callbacks. `receiveCAN()` consumes the resulting software queue.

**Always continue calling `receiveCAN()` in your application loop or task.** Interrupts signal or enqueue availability; the common API does not automatically call your application-level message-processing function.

## 7. Common API

| Method | Purpose |
|---|---|
| `init()` | Initialize the controller and bus bitrate |
| `init_irq()` | Enable backend-specific receive interrupt/notification handling |
| `can_isr()` | Backend-specific interrupt/notification dispatch hook |
| `transmitCAN(id, data, length)` | Request transmission of an 11-bit data frame |
| `receiveCAN(buffer, 4, 8)` | Retrieve one pending frame without blocking |
| `checkStatus(&byte)` | Get backend-specific status bits |
| `reset()` | Reset or restart the CAN controller (backend-dependent) |
| `filtersAndMasks(data, length, addr)` | MCP2515-style register write; not portable to ESP32/Teensy |
| `getBaud()` | Return the baud value passed to the base class |

Useful `status` values include `SUCCESS`, `NO_NEW_MESSAGE`, `TX_ID_ERROR`, `TX_PAYLOAD_ERROR`, `TX_STATUS_STALL`, `TX_STATUS_ERROR`, `RX_ID_ERROR`, `RX_PAYLOAD_ERROR`, `RX_STATUS_ERROR`, and `INIT_ERROR`. Full enum values are in `canlib.h`.

## 8. Known limitations

1. **Standard IDs only:** The API currently uses 11-bit IDs (`0x000`–`0x7FF`) and rejects extended frames. CAN 2.0 also supports 29-bit IDs, but that functionality is not exposed here.
2. **No CAN FD:** At most 8 payload bytes per frame.
3. **RP2350 bitrate configuration:** The Pico implementation's `XL2515::NORMAL_CNF1/2/3` values, not `getBaud()`, determine the CAN bitrate. Verify the constants for the board oscillator and desired bitrate.
4. **Filtering is not portable:** ESP32 and Teensy implementations currently return `SET_CONTROL_BITS_ERROR` for `filtersAndMasks()`.
5. **Status bytes differ:** `checkStatus()` is not a portable status-bit format. Interpret it according to the backend.
6. **Interrupt context matters:** Do not call SPI operations from the RP2350 GPIO ISR or call Teensy's `can.events()` directly from a hardware ISR.
7. **Reset behavior differs:** Reset/restart may clear receive queues and interrupt state; check your backend before relying on it.
8. **Not yet validated:** Compile and exercise each target with its SDK/framework version, CAN transceiver, and a second CAN node before production use.

## 9. Troubleshooting

- **Missing `CANDevice.h`:** Confirm the library is in `lib/CANLib/` or in `lib_deps`, and that `library.json` points to `src`.
- **Wrong backend compiled:** Check that exactly one `CAN_TARGET_*` macro is defined in the selected environment and that `select_platform.py` applied the source filter.
- **Missing `FlexCAN_T4.h`:** Install FlexCAN_T4 via the Teensy environment's `lib_deps`.
- **Missing `driver/twai.h`:** Use an ESP32 framework version containing the legacy TWAI driver, or port the backend to your framework's newer API.
- **No received frames:** Verify the bus bitrate, transceiver power and logic levels, TX/RX connections, shared ground, termination, and presence of a second functioning CAN node.
- **`TX_STATUS_STALL`:** The transmit queue/buffer is busy. Retry later rather than continuously spinning.

## 10. Suggested next improvements

For a cleaner cross-platform API, consider replacing `filtersAndMasks()` and the backend-specific `checkStatus()` byte with portable filter and controller-state types. You may also want to add a user-registered receive callback if you want application code to process messages on notifications instead of polling `receiveCAN()`.
