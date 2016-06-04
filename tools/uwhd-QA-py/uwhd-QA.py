#!/usr/bin/env python

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
    response = raw_input(question.rjust(60, ' ') + ' [Y/N] ')
    if response in valid_responses:
      return valid_responses[response]

    print 'Invalid input'

def instruct_and_wait(instructions):
  bar = colored('**********************************'
                '**********************************',
                'magenta')
  raw_input(bar + '\n' + instructions + '\n' + bar)

def display_checks(mgr, is_front):
  s = True
  mgr.setGameClockRunning(False)
  mgr.setBlackScore(1)
  mgr.setWhiteScore(4)
  mgr.setGameClock(135)
  s = ask_user_yes_no('Are all three displays on?') and s

  # Quit early, there's no point in asking more questions'
  if not s:
    return False

  mgr.setGameStateWallClock()
  s = ask_user_yes_no('Is the displayed wall-clock time correct?') and s

  mgr.setGameClockRunning(True)
  time.sleep(6)
  mgr.setGameClockRunning(False)
  s = ask_user_yes_no('Did the clock count down, and then stop?') and s

  for i in range(0, 100):
    mgr.setWhiteScore(i)
    mgr.setBlackScore(i)
    time.sleep(0.3)
  s = ask_user_yes_no('Did the scores cycle through every number 0-99?') and s

  mgr.setGameClock(102)
  s = ask_user_yes_no('Does the time show 1:42?') and s

  mgr.setGameClock(60)
  mgr.setGameStateBlackTimeOut()
  mgr.setGameClockRunning(True)
  time.sleep(12)
  mgr.setGameClockRunning(False)
  s = ask_user_yes_no('Did the display show a BLACK timeout?') and s

  mgr.setGameClock(60)
  mgr.setGameStateWhiteTimeOut()
  mgr.setGameClockRunning(True)
  time.sleep(12)
  mgr.setGameClockRunning(False)
  s = ask_user_yes_no('Did the display show a WHITE timeout?') and s

  mgr.setGameClock(60)
  mgr.setGameStateRefTimeOut()
  mgr.setGameClockRunning(True)
  time.sleep(12)
  mgr.setGameClockRunning(False)
  s = ask_user_yes_no('Did the display show a REF timeout?') and s

  mgr.setGameClock(2*60)
  mgr.setGameStateHalfTime()
  mgr.setGameClockRunning(True)
  time.sleep(12)
  mgr.setGameClockRunning(False)
  s = ask_user_yes_no('Did the display show HALF TIME?') and s

  mgr.setGameClock(3*60)
  mgr.setGameStateGameOver()
  mgr.setGameClockRunning(True)
  time.sleep(12)
  mgr.setGameClockRunning(False)
  s = ask_user_yes_no('Did the display show GAME OVER?') and s

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

  s = True

  instruct_and_wait('The following checks will ask about the 32x32 panels on\n'
                    'the front of the clock.\n'
                    'Press \'return\' to continue.')
  s = display_checks(mgr, is_front=True) and s

  if s:
    status = colored('[PASSED]', 'green')
  else:
    status = colored('[FAILED]', 'red')

  print '\nQA ', status

