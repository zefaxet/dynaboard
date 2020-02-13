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
import codecs

# dictionary for encoding keypresses for ALT+ unicode typing
unicodedict = {
    "+": (chr(4) + chr(0) + chr(87) + chr(0)*5),
    "0": (chr(4) + chr(0) + chr(98) + chr(0)*5),
    "1": (chr(4) + chr(0) + chr(89) + chr(0)*5),
    "2": (chr(4) + chr(0) + chr(90) + chr(0)*5),
    "3": (chr(4) + chr(0) + chr(91) + chr(0)*5),
    "4": (chr(4) + chr(0) + chr(92) + chr(0)*5),
    "5": (chr(4) + chr(0) + chr(93) + chr(0)*5),
    "6": (chr(4) + chr(0) + chr(94) + chr(0)*5),
    "7": (chr(4) + chr(0) + chr(95) + chr(0)*5),
    "8": (chr(4) + chr(0) + chr(96) + chr(0)*5),
    "9": (chr(4) + chr(0) + chr(97) + chr(0)*5),
    "a": (chr(4) + chr(0) + chr(4) + chr(0)*5),
    "b": (chr(4) + chr(0) + chr(5) + chr(0)*5),
    "c": (chr(4) + chr(0) + chr(6) + chr(0)*5),
    "d": (chr(4) + chr(0) + chr(7) + chr(0)*5),
    "e": (chr(4) + chr(0) + chr(8) + chr(0)*5),
    "f": (chr(4) + chr(0) + chr(9) + chr(0)*5)
    }

# opens config file and saves value as keychar
with codecs.open('dynaboardconfig', encoding='utf-8') as config:
    for line in config:
        keychar = line[0]
# convert configured character to string depicting unicode code
# finds decimal Unicode value of character; converts to hex; converts to string
keychar = str(hex(ord(keychar)))
# removes "0x" hex identifier in string
keychar = keychar[2:]
# pads string with 0's to length 4
keychar = "+" + keychar.zfill(4)
 
def keypress(channel):
    # open HID device "file"
    dbrd = open("/dev/hidg0","rb+")
    for letter in keychar:
        dbrd.write(unicodedict[letter].encode())
        dbrd = open("/dev/hidg0","rb+")
    # send "clear" keypress
    dbrd.write((chr(0)*8).encode())
    print("key pressed")
    #p1 = Popen(["sudo echo -ne "+ keys[channel] + " > /dev/hidg0", "sleep .25s", r"sudo echo -ne\0\0\0\0\0\0\0\0 > /dev/hidg0"], shell=True)
    
    
    
# set GPIO numbering mode
GPIO.setmode(GPIO.BCM)

# list of GPIO pins being used for keys, as well as values for those keys
keys = [17]

# sets pins in "keys" list as input pins
GPIO.setup(list(keys), GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

# sets up events on all pins attached to keys
for i in keys:
    GPIO.add_event_detect(i, GPIO.RISING, callback=keypress, bouncetime=150)

message = input("press enter to quit\n\n")
 
# cleans up GPIO pins
GPIO.cleanup()
    
    