import pigpio
import os
from time import sleep

def init():
    os.system("sudo pigpiod")
    sleep(2)
    pi = pigpio.pi()
    pi.setmode(4, PIGPIO.INPUT)
    pi.setmode(18, PIGPIO.OUTPUT)
    pi.setmode(26, PIGPIO.OUTPUT)
    pi.write(4, 1)
    return


def readClicker():
    return pi.read(4)


def setSound(setting):
    pi.write(26, setting)
    return