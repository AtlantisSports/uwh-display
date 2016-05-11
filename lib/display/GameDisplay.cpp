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
    for (unsigned y = 0; y < 32 * 3; y++)
      for (unsigned x = 0; x < 32 * 3; x++)
        Frame->SetPixel(x, y, Background.r, Background.g, Background.b);

    GameModel M = Mgr.getModel();

    TD.Render(Frame);

    if (M.BlackScore < 10)
      // One's
      BigNumber::Render(Frame, 0, M.BlackScore % 10, /*xo=*/10, /*yo=*/1,
                        BigNumber::Font::Digit15x29, BlackTeamFG, &BlackTeamBG);
    else if (10 <= M.BlackScore && M.BlackScore < 100) {
      // Ten's
      BigNumber::Render(Frame, 0, (M.BlackScore / 10) % 10, /*xo=*/0, /*yo=*/1,
                        BigNumber::Font::Digit15x29, BlackTeamFG, &BlackTeamBG);

      // One's
      BigNumber::Render(Frame, 0, M.BlackScore % 10, /*xo=*/17, /*yo=*/1,
                        BigNumber::Font::Digit15x29, BlackTeamFG, &BlackTeamBG);
    }

    if (M.WhiteScore < 10)
      // One's
      BigNumber::Render(Frame, 2, M.WhiteScore % 10, /*xo=*/10, /*yo=*/1,
                        BigNumber::Font::Digit15x29, WhiteTeamFG, &WhiteTeamBG);
    else if (10 <= M.WhiteScore && M.WhiteScore < 100) {
      // Ten's
      BigNumber::Render(Frame, 2, (M.WhiteScore / 10) % 10, /*xo=*/0, /*yo=*/1,
                        BigNumber::Font::Digit15x29, WhiteTeamFG, &WhiteTeamBG);

      // One's
      BigNumber::Render(Frame, 2, M.WhiteScore % 10, /*xo=*/17, /*yo=*/1,
                        BigNumber::Font::Digit15x29, WhiteTeamFG, &WhiteTeamBG);
    }

    Frame = Mtx->SwapOnVSync(Frame);
  }
}
