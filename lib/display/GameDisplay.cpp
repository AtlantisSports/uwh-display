//===-- GameDisplay.cpp - Rendering Manager ------------------------ c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/display/GameDisplay.h"

#include "uwhd/display/BigNumber.h"

#include <graphics.h>
#include <led-matrix.h>

#include <iostream>

using namespace rgb_matrix;

void GameDisplay::Run() {
  FrameCanvas *Frame = Mtx->CreateFrameCanvas();
  while (running()) {
    GameModel M = Mgr.getModel();
    for (unsigned y = 0; y < 32 * 3; y++)
      for (unsigned x = 0; x < 32 * 3; x++)
        Frame->SetPixel(x, y, Background.r, Background.g, Background.b);
    BigNumber::Render(Frame, 0, M.BlackScore, BlackTeamFG, &BlackTeamBG);
    TD.Render(Frame);
    BigNumber::Render(Frame, 2, M.WhiteScore, WhiteTeamFG, &WhiteTeamBG);
    Frame = Mtx->SwapOnVSync(Frame);
  }
}
