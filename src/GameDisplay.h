#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include "GameModel.h"
#include "TimeDisplay.h"

#include <canvas.h>
#include <led-matrix.h>
#include <threaded-canvas-manipulator.h>

namespace rgb_matrix {
  class RGBMatrix;
}

namespace uwhtimer {

class GameDisplay : public rgb_matrix::ThreadedCanvasManipulator {
public:
  GameDisplay(rgb_matrix::RGBMatrix *Mtx)
    : ThreadedCanvasManipulator(Mtx)
    , Mtx(Mtx)
    , Mgr()
    , TD(1, Mgr)
  {}

  void Run() override;

private:
  rgb_matrix::RGBMatrix *Mtx;
  GameModelManager Mgr;
  TimeDisplay TD;
};

}

#endif
