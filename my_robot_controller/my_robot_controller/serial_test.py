import os

def find_serial_port_with_string(string):
    usb_devices_path = "/dev/serial/by-id/"
    for device_file in os.listdir(usb_devices_path):
        if string in device_file:
            device_path = os.path.join(usb_devices_path, device_file)
            device_port = os.path.realpath(device_path)
            if device_port.startswith("/dev/ttyUSB"):
                return device_port
    return None

def main():
    device_string = input("Enter the device string to search for: ")
    device_port = find_serial_port_with_string(device_string)
    if device_port:
        print(f"The device containing the string '{device_string}' is connected to: {device_port}")
    else:
        print(f"No device containing the string '{device_string}' is found or connected.")

if __name__ == "__main__":
    main()


