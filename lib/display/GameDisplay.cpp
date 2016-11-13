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

void renderSingleDigitScore(unsigned X, unsigned Y, unsigned Score,
                            UWHDPixel FG, const UWHDPixel *BG,
                            UWHDCanvas *C) {
  assert(Score < 10);
  // One's
  BigNumber::Render(C, 0, Score % 10, /*xo=*/X, /*yo=*/Y,
                    BigNumber::Font::Digit15x29, FG, BG);
}

void renderDoubleDigitScore(unsigned X, unsigned Y, unsigned Score,
                            UWHDPixel FG, const UWHDPixel *BG,
                            UWHDCanvas *C) {
  assert(Score < 100);
  // Ten's
  BigNumber::Render(C, 0, (Score / 10) % 10, /*xo=*/X, /*yo=*/Y,
                    BigNumber::Font::Digit15x29, FG, BG);

  // One's
  BigNumber::Render(C, 0, Score % 10, /*xo=*/X+17, /*yo=*/Y,
                    BigNumber::Font::Digit15x29, FG, BG);
}

void renderScore(unsigned Display, unsigned Score,
                 UWHDPixel FG, const UWHDPixel *BG,
                 UWHDCanvas *C) {
  if (Score < 10)
    renderSingleDigitScore(32 * Display + 10, 1, Score, FG, BG, C);
  else if (10 <= Score && Score < 100)
    renderDoubleDigitScore(32 * Display +  0, 1, Score, FG, BG, C);
}

void renderGameDisplay(unsigned Version, GameModel M, UWHDCanvas *C) {
  C->fill(UWHDBackground);

  switch (Version) {
  case 1:
    renderTimeDisplay(M, C);

    if (M.State != GameModel::WallClock) {
      renderScore(0, M.BlackScore, UWHDBlackTeamFG, &UWHDBlackTeamBG, C);
      renderScore(2, M.WhiteScore, UWHDWhiteTeamFG, &UWHDWhiteTeamBG, C);
    }
    break;
  default:
    break;
  }
}
