# 11_PolledEcho

## Project Description

This project implements a simple echo program on an STM32G431RBTX microcontroller. The goal is to receive a string via the serial interface and immediately send the same string back (echo functionality).

## Hardware
- STM32G431RBTX microcontroller

## Structure
- The firmware is located in the `Core/` and `User_App/` folders.
- The file `STM32G431RBTX_FLASH.ld` contains the linker script.
- Project configuration is managed via `11_PolledEcho.ioc` (STM32CubeMX).
- The main program is executed from the UserMain files within the User_App folder.

## Testing and Communication
The `test/` folder contains the Python script `send_serial_command.py`, which can be used to send a string to the target and verify the echo response.

### Using the Python Test Script

1. Set up the Python environment:
   ```bash
   python3 -m venv venv
   source venv/bin/activate
   pip install -r requirements.txt
   ```

2. Run the script:
   ```bash
   python test/send_serial_command.py
   ```

The script sends a string via the serial interface and prints the microcontroller's response.

## Build and Flash

This project can be built and flashed with the CubeIDE.

## Dependencies
- STM32CubeIDE
- Python (for the test script)

