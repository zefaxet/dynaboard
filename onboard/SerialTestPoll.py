import serial

# follow instructions here: https://learn.adafruit.com/turning-your-raspberry-pi-zero-into-a-usb-gadget/serial-gadget
# in order to set up Pi as serial device

# instantiates the serial port on the pi
serialport = serial.Serial("/dev/ttyGS0", 9600, timeout=0.5)

# listens on the serial port for incoming commands and prints
while True:    
    command = serialport.read()
    print str(command)
