# RUN: python %s | display-check -

import uwhd

def main():
  version = 1
  black_score = 3
  white_score = 5
  time = 42

  mgr = uwhd.GameModelManager()

  print('# SET-VERSION: %d' % (version,))
  print('# SET-STATE: FirstHalf')
  print('# SET-BLACK: %d' % (black_score,))
  print('# SET-WHITE: %d' % (white_score,))
  print('# SET-TIME: %d' % (time,))

  mgr.setGameStateFirstHalf()
  mgr.setBlackScore(black_score)
  mgr.setWhiteScore(white_score)
  mgr.setGameClock(time)

  canvas = uwhd.UWHDCanvas.create(32 * 3, 32)
  uwhd.renderGameDisplay(1, mgr.getModel(), canvas)
  ppmstr = uwhd.asPPMString(canvas)

  print(ppmstr)

if __name__ == '__main__':
  main()
