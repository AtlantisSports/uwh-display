#!/usr/bin/env python3

import uwhd
from overlay.ui import Overlay, MaskKind

def main():
  print("Starting GameModelManager")
  mgr = uwhd.GameModelManager()

  print("Starting xbee comms...")
  xbee = uwhd.CreateXBeeSyncServer()
  xbee.Init()
  xbee.setMgr(mgr)

  mgr.setGameStateFirstHalf()
  mgr.setGameClockRunning(0)
  mgr.setBlackScore(0)
  mgr.setWhiteScore(0)
  mgr.setGameClock(0)

  ov = Overlay(mgr, "chroma", "left")
  ov.mainloop()

if __name__=="__main__":
  main()
