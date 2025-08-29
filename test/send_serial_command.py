

import serial
import serial.tools.list_ports
import time

# ------------------- Constants -------------------
BAUDRATE = 115200
SERIAL_TIMEOUT = 100  # in seconds
DOTS = ['', '.', '..', '...']
STATUS_BASE_TEXT = "Waiting for Target Output -> "
POLL_INTERVAL = 0.5  # in seconds

# ------------------- Functions -------------------
def list_serial_ports():
    all_ports = list(serial.tools.list_ports.comports())
    # Build a list of (original_index, port) for ports without 'n/a'
    usable_ports = [(idx, port) for idx, port in enumerate(all_ports) if 'n/a' not in port.description.lower()]
    if not usable_ports:
        print("No usable serial ports found.")
        exit(1)
    print("Available serial ports:")
    for orig_idx, port in usable_ports:
        print(f"[{orig_idx}] {port.device} - {port.description}")
    return usable_ports

def select_serial_port(usable_ports):
    orig_indices = [orig_idx for orig_idx, _ in usable_ports]
    while True:
        try:
            selection = int(input("Select port number: "))
            for orig_idx, port in usable_ports:
                if selection == orig_idx:
                    return port.device
            print("Invalid selection.")
        except ValueError:
            print("Please enter a number.")

def get_command():
    return input("Enter the string to send (will be terminated with newline): ") + '\n'

def send_command(serial_port, command):
    try:
        with serial.Serial(serial_port, BAUDRATE, timeout=SERIAL_TIMEOUT) as ser:
            ser.write(command.encode('utf-8'))
            print(f"Sent: {command.strip()} to {serial_port}")

            idx = 0
            try:
                while True:
                    line = f"{STATUS_BASE_TEXT}{DOTS[idx]}"
                    print(f"\r{line}", end='', flush=True)
                    time.sleep(POLL_INTERVAL)
                    idx = (idx + 1) % len(DOTS)
                    if idx == 0:
                        # Clear line dynamically based on text length
                        print("\r" + " " * len(line) + "\r", end='', flush=True)
                    if ser.in_waiting:
                        response = ser.read(ser.in_waiting)
                        decoded_response = response.decode('utf-8', errors='replace').strip()
                        print("\r" + " " * len(line) + "\r", end='', flush=True)
                        print(f"Response: {decoded_response}")
                        break
            except KeyboardInterrupt:
                print("\nPolling aborted by user.")
    except serial.SerialException as e:
        print(f"Warning: Could not open serial port {serial_port}. Reason: {e}")

def main():
    ports = list_serial_ports()
    serial_port = select_serial_port(ports)
    print("Press Ctrl+C to exit.")
    try:
        while True:
            command = get_command()
            send_command(serial_port, command)
    except KeyboardInterrupt:
        print("\nExiting.")

# ------------------- Entry Point -------------------
if __name__ == "__main__":
    main()

