# Dev workflow (CLI)

This folder contains maintainer-only sketches and scripts to build, upload, and monitor devices **without the Arduino IDE**.

## Prereqs
- **Arduino CLI** available on your PATH (`arduino-cli version`).
- **Cores installed**: `arduino:avr` and `esp32-bluepad32:esp32` (run `./dev/setup.ps1`).
- (Optional) On Windows, you may need to allow scripts once in an elevated PowerShell:
  ```powershell
  Set-ExecutionPolicy RemoteSigned
  ```

---

## One-time setup
Initializes Arduino CLI, adds the Bluepad32 board index, and installs required cores.
```powershell
.\dev\Setup.ps1
```

---

## List available ports
Shows connected boards and COM ports.
```powershell
.\dev\ListPorts.ps1
```

---

## Build + upload + auto-monitor (receiver)
ATmega2560 side (builds against this repo via `--libraries .`):
```powershell
.\dev\UploadRecv.ps1 -Port COM3
```
**Options**
```powershell
# Change baud / board target if needed
.\dev\UploadRecv.ps1 -Port COM3 -Baud 230400 -Fqbn "arduino:avr:mega"
```

---

## Build + upload + auto-monitor (sender)
ESP32 side using the **Bluepad32** board package:
```powershell
.\dev\UploadSnd.ps1 -Port COM5
```
**Options**
```powershell
# Different ESP32 target (example: S3)
.\dev\UploadSnd.ps1 -Port COM5 -Fqbn "esp32-bluepad32:esp32:esp32s3"
```

---

## Tips
- Exit serial monitor with **Ctrl+C**.
- Some ESP32 boards re-enumerate after flashing. If the COM port number changes, re-run the sender script with the new port.
- Keep includes as `<XBoxControllerHandler.h>` and `<Bluepad32.h>`; the scripts pass `--libraries .` so your **local** `src/` is used during compilation.

---

## Troubleshooting
- **Unknown core ID / cannot install Bluepad32 core**  
  Re-run setup and make sure the Bluepad32 index is present:
  ```powershell
  .\dev\Setup.ps1
  arduino-cli core list
  ```
- **`Access denied` opening COM port**  
  Close other apps/IDE/monitors. Unplug/replug the board. Check Device Manager for the correct COM number.
- **Upload fails (AVR)**  
  Verify the board and port. Press reset just before upload if needed.
- **Upload fails / port disappears (ESP32)**  
  Some boards switch ports after flashing; re-run with the new COM port. Ensure drivers (e.g., CP210x/CH34x) are installed.
- **Build can’t find Bluepad32**  
  Ensure you’re compiling the **sender** with an `esp32-bluepad32:esp32:*` FQBN. The Bluepad32 API for ESP32 comes from the **board package**, not a Library Manager lib.

---

## File layout (reference)
```
dev/
├─ Atmega_Recv/
│   └─ Atmega_Recv.ino
├─ ESP32_Snd/
│   └─ ESP32_Snd.ino
├─ Setup.ps1            # install cores & indexes
├─ ListPorts.ps1        # show ports
├─ UploadRecv.ps1       # build/upload/monitor AVR receiver
└─ UploadSnd.ps1        # build/upload/monitor ESP32 sender
```
