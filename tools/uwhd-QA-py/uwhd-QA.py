#!/bin/env python

import sys
import uwhd
import time
from termcolor import colored


def ask_user_yes_no(question):
  valid_responses = {
    'y'   : True,
    'Y'   : True,
    'yes' : True,
    'Yes' : True,
    'n'   : False,
    'N'   : False,
    'no'  : False,
    'No'  : False
  }

  while True:
    response = raw_input(question + ' [Y/N] ')
    if response in valid_responses:
      return valid_responses[response]

    print 'Invalid input'

def instruct_and_wait(instructions):
  bar = colored('*************************************************', 'magenta')
  raw_input(bar + '\n' + instructions + '\n' + bar)

def display_checks(mgr, is_front):
  s = True
  mgr.setClockRunning(False)
  mgr.setBlackScore(1)
  mgr.setWhiteScore(4)
  mgr.setGameClockSecs(135)
  s = ask_user_yes_no('Are all three displays on?') and s

  # Quit early, there's no point in asking more questions'
  if not s:
    return False

  mgr.setClockRunning(True)
  time.sleep(2)
  mgr.setClockRunning(False)
  s = ask_user_yes_no('Did the clock count down, and then stop?') and s

  mgr.setBlackScore(99)
  s = ask_user_yes_no('Is the black score 99?') and s

  mgr.setWhiteScore(99)
  s = ask_user_yes_no('Is the white score 99?') and s

  mgr.setWhiteScore(0)
  mgr.setBlackScore(0)
  s = ask_user_yes_no('Are both scores 0?') and s

  mgr.setGameClockSecs(102)
  s = ask_user_yes_no('Does the time show 1:42?') and s

  return s

if __name__ == '__main__':
  io = uwhd.GPIO()
  if not io.Init():
    print 'Try running this as:'
    print ''
    print '$ sudo PYTHONPATH=path/to/build/lib python uwhd-QA.py'
    sys.exit(-1)

  matrix = uwhd.RGBMatrix(io, 32, 3, 1)
  gd = uwhd.GameDisplay(matrix)
  gd.Start0()
  mgr = gd.getMgr2()
  mgr.setClockRunning(False)
  mgr.setBlackScore(0)
  mgr.setWhiteScore(0)
  mgr.setGameClockSecs(1)

  s = True

  instruct_and_wait('The following checks will ask about the 32x32 panels on\n'
                    'the front of the clock.\n'
                    'Press \'return\' to continue.')
  s = display_checks(mgr, is_front=True) and s

  instruct_and_wait('The following checks will ask about the alphanumeric\n'
                    'displays on the back of the clock.\n'
                    'Press \'return\' to continue.')
  s = display_checks(mgr, is_front=False) and s

  if s:
    status = colored('[PASSED]', 'green')
  else:
    status = colored('[FAILED]', 'red')

  print '\nQA ', status

