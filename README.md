# Arduino-XBox-Controller-Handler

Parse controller state strings sent from an **ESP32 (Bluepad32)** and expose **easy change‑callbacks** on an Arduino‑compatible MCU with more I/O (e.g., ATmega2560). This library is intended for the **Arduino IDE** and also works great with **Arduino CLI**.

> **Why?** Many projects want wireless controller input but still need lots of pins, relays, or precise timing on a separate MCU. Pair an ESP32 (controller stack) with a Mega/other MCU (I/O + control) over UART.

---

## Features
- **Change handlers**: provide function pointers that fire when a controller state changes (handlers can vary by controller index).
- **Up to 4 controllers** via `controllerCount`.
- **Single configuration struct** (`ControllerManagerConfig`) to set the UART and library settings in one place.
- Works with strings formatted by an ESP32 sender (Bluepad32) — **format documented below**.
- Tested with **ESP32‑C3 WROOM → ATmega2560 (Serial1)**.

While Bluepad32 supports many controllers, this library focuses on **Xbox** controllers. Other devices may work if the sender outputs the same field set; for very different devices, treat this library as a **template parser**.

---

## Architecture
- **Sender (ESP32)**: reads the Xbox controller with [Bluepad32](https://github.com/ricardoquesada/bluepad32) and emits one ASCII line per update on a UART channel.
- **Receiver (Arduino/AVR/other)**: runs this library, parses each line, and invokes your change handlers.

---

## Installation
- **Manual**: clone or download this repo into your Arduino `libraries/` folder, or add as a ZIP library.
- **Library Manager**: if listed, install via the Library Manager menu that is integrated into the Arduino IDE (search for "XBoxControllerHandler").

> For CLI‑only development, see **Development (CLI)** below.

---

## Quick start (receiver)
```cpp
#include <XBoxControllerHandler.h>

void onButtonAChanged(uint8_t idx, bool down) {
  Serial.print("Controller "); Serial.print(idx);
  Serial.println(down ? ": A pressed" : ": A released");
}

void setup() {
  Serial.begin(115200);

  ControllerManagerConfig cfg;
  cfg.receiverSerial = &Serial1;   // UART connected to the ESP32 sender
  cfg.baud = 115200;
  cfg.controllerCount = 1;

  ControllerManager::begin(cfg);
  ControllerManager::onButtonAChanged(onButtonAChanged);
}

void loop() {
  // Non‑blocking parser: read available bytes and dispatch change events
  ControllerManager::poll();
}
```

---

## Wiring (example)
| ESP32‑C3 WROOM (sender) | ATmega2560 (receiver) | Notes |
|---|---|---|
| TX (pin 12)        | RXD2 (pin 12)         | UART link carrying controller frames |
| GND                | GND                   | Common ground                        |

- If needed, use some form of logic level shifting - voltage divider or discrete IC (TI TXS0108ERGYR).
- Match the **baud** and UART in `ControllerManagerConfig` to your wiring.

---

## Protocol (gamepad frames from sender → receiver)
The ESP32 sender (Bluepad32) emits **one line per update**, terminated by `\n`, using a **pipe‑delimited** format produced by `dumpGamepad(...)` in the ESP32 example:

```
DATA|<idx>|<dpad>|<buttons>|<axisX>|<axisY>|<axisRX>|<axisRY>|<brake>|<throttle>|<miscButtons>
```

**Observed example (from receiver debug):**
```
DATA|0|0|0|-8|-15|-3|-8|0|0|0
```

**Field reference**

| Field           | Type    | Notes / Range                                                                    |
|-----------------|---------|-----------------------------------------------------------------------------------|
| `DATA`          | literal | Frame prefix                                                                      |
| `<idx>`         | int     | Controller index                                                                 |
| `<dpad>`        | int     | D‑pad bitfield                                                                    |
| `<buttons>`     | int     | Buttons bitmask                                                                   |
| `<axisX>`       | int     | Left X axis (typically **−511..512**)                                             |
| `<axisY>`       | int     | Left Y axis (typically **−511..512**)                                             |
| `<axisRX>`      | int     | Right X axis (typically **−511..512**)                                            |
| `<axisRY>`      | int     | Right Y axis (typically **−511..512**)                                            |
| `<brake>`       | int     | **0..1023**                                                                       |
| `<throttle>`    | int     | **0..1023**                                                                       |
| `<miscButtons>` | int     | Misc buttons bitmask                                                              |

> The sender’s **debug** `Serial.printf` also logs gyro/accel values, but those are **not** transmitted in the UART frame above.

If you change the sender format (names/order/ranges), update the receiver’s parser accordingly.

---

## Debugging raw frames
For troubleshooting, you can print the raw frame string the receiver sees. In `XBoxControllerHandler::ProcessSerial` you can enable the existing debug prints, e.g.:

```cpp
// Example snippet inside ProcessSerial:
m_processorSerial->print("Recv: '");
m_processorSerial->print(data);
m_processorSerial->print("'\n");
```

This lets you confirm the exact frame received matches the documented protocol.

---

## API overview (selected)
- `ControllerManager::begin(const ControllerManagerConfig&)`
- `ControllerManager::poll()`
- Change handlers (examples):
  - `onButtonAChanged(fn)`, `onButtonBChanged(fn)`
  - `onLeftAxisChanged(fn(int16_t x, int16_t y))`
  - `onRightAxisChanged(fn(int16_t x, int16_t y))`
  - `onTriggerChanged(fn(uint16_t lt, uint16_t rt))`

See headers and examples for the complete set.

---

## Examples
- **InputPrintout** — prints any detected changes to `Serial` (see `examples/`).
- **Development** — sender/receiver sketches for CLI iteration (see `dev/ESP32_Snd` and `dev/Atmega_Recv`).

---

## ESP32 sender (Bluepad32)
The sender reads the Xbox controller via Bluepad32 and emits the frame above on a UART channel. Include and use:
```cpp
#include <Bluepad32.h>
```
Install the **ESP32 + Bluepad32** board package and select the appropriate FQBN (e.g., `esp32-bluepad32:esp32:esp32c3`).

---

## Development (CLI)
This repo includes PowerShell scripts under **`dev/`** to enable fast, IDE‑free iteration that always builds against the **local library source**:
- `Setup.ps1` — installs board indexes/cores with fallbacks
- `ListPorts.ps1` — list available COM ports
- `UploadRecv.ps1` / `UploadSnd.ps1` — **compile → upload → auto‑open serial monitor**

Example:
```powershell
# One‑time
.\dev\Setup.ps1

# As needed to determine the correct COM port
.\dev\ListPorts.ps1

# Receiver (ATmega2560) on COM3
.\dev\UploadRecv.ps1 -Port COM3

# Sender (ESP32‑C3) on COM5
.\dev\UploadSnd.ps1 -Port COM5
```

---

## License
MIT
