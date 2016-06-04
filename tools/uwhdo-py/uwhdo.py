#!/usr/bin/env python

import sys
import uwhdo
import time
from termcolor import colored


if __name__ == '__main__':
  coordinator = False
  xbee_sync = uwhdo.CreateXBeeSync(coordinator)

  mgr = xbee_sync.getMgr()

  s = True


