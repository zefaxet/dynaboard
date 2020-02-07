#####################################################
#  Dynaboard Demo v1.0
#
#  Description: receives button-press from GPIO pins
#               and sends preset keypress to host PC
#
#  Credit:  GPIO code adapted from Paper Piano
#           activity by Gourd & Irteza
#####################################################

import RPi.GPIO as GPIO
from time import sleep

 
def keypress(channel):
    # open HID device "file"
    dbrd = open("/dev/hidg0","rb+")
    dbrd.write(keys[channel].encode())
    dbrd = open("/dev/hidg0","rb+")
    dbrd.write((chr(0)*8).encode())
    print("key pressed")
    #p1 = Popen(["sudo echo -ne "+ keys[channel] + " > /dev/hidg0", "sleep .25s", r"sudo echo -ne\0\0\0\0\0\0\0\0 > /dev/hidg0"], shell=True)
    
    
    
# set GPIO numbering mode
GPIO.setmode(GPIO.BCM)

# list of GPIO pins being used for keys, as well as values for those keys
keys = {
    17: (chr(0)*2 + chr(4) + chr(0)*5)
    }

# sets pins in "keys" list as input pins
GPIO.setup(list(keys), GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

# sets up events on all pins attached to keys
for i in keys:
    GPIO.add_event_detect(i, GPIO.RISING, callback=keypress, bouncetime=75)

message = input("press enter to quit\n\n")
 
# cleans up GPIO pins
GPIO.cleanup()
    
    