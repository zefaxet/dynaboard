import serial

# instantiates the serial port on the host pc
# on Windows, port name is listed under device manager - Ports>PiUSB to Serial
ser = serial.Serial("COM3", 9600)

# sends command over serial port
command = b'\x41\x42\x43\x44\x48\x65\x6c\x6c\x6f'
ser.write(command)
