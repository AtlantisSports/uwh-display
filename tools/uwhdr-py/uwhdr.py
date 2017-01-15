#!/usr/bin/env python3

import time
import sys
import uwhd
try:
  import pigpio
  NO_PIGPIO = False
except ImportError as e:
  NO_PIGPIO = True
import os
import configparser
from refbox.ui import NormalView

config = configparser.RawConfigParser()
config.read('/home/pi/workdir/uwh-display/tools/uwhdr-py/game.cfg')
HALF_PLAY_DURATION = config.getint('game', 'half_play_duration')
HALF_TIME_DURATION = config.getint('game', 'half_time_duration')
GAME_OVER_DURATION = config.getint('game', 'game_over_duration')
NO_TITLE_BAR = True

class IOManager(object):
  def __init__(self):
    os.system("sudo pigpiod")
    sys.stdout.write("Initing IO")
    sys.stdout.flush()
    for i in range(1, 5):
      time.sleep(1)
      sys.stdout.write(".%d" %(4-i,))
      sys.stdout.flush()

    self.io = pigpio.pi()
    self.io.set_mode(4, pigpio.INPUT)
    self.io.set_pull_up_down(4, pigpio.PUD_UP)
    self.io.set_mode(18, pigpio.OUTPUT)
    self.io.set_mode(26, pigpio.OUTPUT)

  def turnOnWetDisplays(self):
    self.io.write(4, 1)

  def readClicker(self):
    return not self.io.read(4)

  def setSound(self, setting):
    self.io.write(26, setting)

class NOIOManager(object):
  def __init__(self):
    pass

  def turnOnWetDisplays(self):
    pass

  def readClicker(self):
    return False

  def setSound(self, setting):
    pass

def main():
  print("Starting gpio...")
  iomgr = NOIOManager() if NO_PIGPIO else IOManager()

  print("Turning on wet displays")
  iomgr.turnOnWetDisplays()

  print("Starting xbee comms...")
  mgr = uwhd.GameModelManager()
  if not NO_PIGPIO:
    print("starting xbee")
    xbee = uwhd.CreateXBeeSyncServer()
    xbee.Init()
    xbee.setMgr(mgr)

  mgr.setGameStateFirstHalf()
  mgr.setGameClockRunning(0)
  mgr.setBlackScore(0)
  mgr.setWhiteScore(0)
  mgr.setGameClock(HALF_PLAY_DURATION)

  nv = NormalView(mgr, iomgr, NO_TITLE_BAR)
  nv.root.mainloop()

if __name__=="__main__":
  main()
