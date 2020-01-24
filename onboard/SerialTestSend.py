import serial

ser = serial.Serial("COM3", 9600)
command = b'\x41\x42\x43\x44\x48\x65\x6c\x6c\x6f'
ser.write(command)
