
import serial
import serial.tools.list_ports
import time

# ----------------------------------------------------------------

# List all available serial ports
ports = list(serial.tools.list_ports.comports())
if not ports:
    print("No serial ports found.")
    exit(1)

print("Available serial ports:")
for idx, port in enumerate(ports):
    print(f"[{idx}] {port.device} - {port.description}")

# ----------------------------------------------------------------

# User selection
while True:
    try:
        selection = int(input("Select port number: "))
        if 0 <= selection < len(ports):
            SERIAL_PORT = ports[selection].device
            break
        else:
            print("Invalid selection.")
    except ValueError:
        print("Please enter a number.")

# ----------------------------------------------------------------

BAUDRATE = 115200
COMMAND = input("Enter the string to send (will be terminated with newline): ") + '\n'

with serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1) as ser:
    ser.write(COMMAND.encode('utf-8'))
    print(f"Sent: {COMMAND.strip()} to {SERIAL_PORT}")

    dots = ['', '.', '..', '...']
    idx = 0
    base_text = "Waiting for Target Output -> "
    try:
        while True:
            line = f"{base_text}{dots[idx]}"
            print(f"\r{line}", end='', flush=True)
            time.sleep(0.5)
            idx = (idx + 1) % 4
            if idx == 0:
                # Clear line dynamically based on text length
                print("\r" + " " * len(line) + "\r", end='', flush=True)
            if ser.in_waiting:
                response = ser.read(ser.in_waiting)
                print(f"\rResponse: {response}")
                break
    except KeyboardInterrupt:
        print("\nPolling aborted by user.")
