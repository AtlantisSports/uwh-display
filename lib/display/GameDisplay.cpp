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

#include <iostream>

const UWHDPixel UWHDBackground   = UWHDPixel(  0,   0,   0);
const UWHDPixel UWHDWhiteTeamFG  = UWHDPixel(255, 255, 255);
const UWHDPixel UWHDBlackTeamFG  = UWHDPixel(  0,   0, 255);
const UWHDPixel UWHDWhiteTeamBG  = UWHDBackground;
const UWHDPixel UWHDBlackTeamBG  = UWHDBackground;

void renderGameDisplay(GameModel M, UWHDCanvas *C) {
  C->fill(UWHDBackground);

  renderTimeDisplay(M, C);

  if (M.State != GameModel::WallClock) {
    if (M.BlackScore < 10)
      // One's
      BigNumber::Render(C, 0, M.BlackScore % 10, /*xo=*/10, /*yo=*/1,
                        BigNumber::Font::Digit15x29, UWHDBlackTeamFG, &UWHDBlackTeamBG);
    else if (10 <= M.BlackScore && M.BlackScore < 100) {
      // Ten's
      BigNumber::Render(C, 0, (M.BlackScore / 10) % 10, /*xo=*/0, /*yo=*/1,
                        BigNumber::Font::Digit15x29, UWHDBlackTeamFG, &UWHDBlackTeamBG);

      // One's
      BigNumber::Render(C, 0, M.BlackScore % 10, /*xo=*/17, /*yo=*/1,
                        BigNumber::Font::Digit15x29, UWHDBlackTeamFG, &UWHDBlackTeamBG);
    }

    if (M.WhiteScore < 10)
      // One's
      BigNumber::Render(C, 2, M.WhiteScore % 10, /*xo=*/10, /*yo=*/1,
                        BigNumber::Font::Digit15x29, UWHDWhiteTeamFG, &UWHDWhiteTeamBG);
    else if (10 <= M.WhiteScore && M.WhiteScore < 100) {
      // Ten's
      BigNumber::Render(C, 2, (M.WhiteScore / 10) % 10, /*xo=*/0, /*yo=*/1,
                        BigNumber::Font::Digit15x29, UWHDWhiteTeamFG, &UWHDWhiteTeamBG);

      // One's
      BigNumber::Render(C, 2, M.WhiteScore % 10, /*xo=*/17, /*yo=*/1,
                        BigNumber::Font::Digit15x29, UWHDWhiteTeamFG, &UWHDWhiteTeamBG);
    }
  }
}
