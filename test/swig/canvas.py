# RUN: python3 %s | display-check -

import uwhd

def main():
  version = 1
  black_score = 3
  white_score = 5
  time = 42

  # Build a GameModelManager with a known state:
  mgr = uwhd.GameModelManager()
  mgr.setGameStateFirstHalf()
  mgr.setBlackScore(black_score)
  mgr.setWhiteScore(white_score)
  mgr.setGameClock(time)

  # Render that state to a canvas:
  canvas = uwhd.UWHDCanvas.create(32 * 3, 32)
  uwhd.renderGameDisplay(version, mgr.getModel(), canvas)

  # Print out the expected and actual display state
  # for display-check to verify:
  print('# SET-VERSION: %d' % (version,))
  print('# SET-STATE: FirstHalf')
  print('# SET-BLACK: %d' % (black_score,))
  print('# SET-WHITE: %d' % (white_score,))
  print('# SET-TIME: %d' % (time,))
  print(uwhd.asPPMString(canvas))

if __name__ == '__main__':
  main()
