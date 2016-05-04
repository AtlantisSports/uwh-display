//===-- GameDisplay.h - Rendering Manager -------------------------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include "uwhd/display/TimeDisplay.h"
#include "uwhd/model/GameModel.h"

#include <graphics.h>
#include <canvas.h>
#include <led-matrix.h>
#include <threaded-canvas-manipulator.h>

namespace rgb_matrix {
  class RGBMatrix;
}

class GameDisplay : public rgb_matrix::ThreadedCanvasManipulator {
public:
  GameDisplay(rgb_matrix::RGBMatrix *Mtx)
    : ThreadedCanvasManipulator(Mtx)
    , Mtx(Mtx)
    , Mgr()
    , TD(1, Mgr)
  {}

  void Run();

  GameModelManager &getMgr() { return Mgr; }

private:
  rgb_matrix::RGBMatrix *Mtx;
  GameModelManager Mgr;
  TimeDisplay TD;

public:
  static const rgb_matrix::Color WhiteTeamFG;
  static const rgb_matrix::Color WhiteTeamBG;
  static const rgb_matrix::Color BlackTeamFG;
  static const rgb_matrix::Color BlackTeamBG;
  static const rgb_matrix::Color Background;
};

#endif
