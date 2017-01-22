//===-- TimeDisplay.cpp - Renders a 32x32 Time Panel --------------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/display/TimeDisplay.h"

#include "uwhd/display/BigNumber.h"
#include "uwhd/model/GameModel.h"

#include <ctime>

const UWHDPixel UWHDSecondsColor = UWHDPixel(  0, 255,   0);
const UWHDPixel UWHDMinutesColor = UWHDPixel(  0, 255,   0);
const UWHDPixel UWHDColonColor   = UWHDPixel(  0, 255,   0);
const UWHDPixel UWHDBackground   = UWHDPixel(  0,   0,   0);
const UWHDPixel UWHDLogoColor1   = UWHDPixel(160,   0, 255);
const UWHDPixel UWHDLogoColor2   = UWHDPixel( 80,   0, 255);
const UWHDPixel UWHDWhiteTimeoutColor = UWHDPixel(255, 255, 255);
const UWHDPixel UWHDBlackTimeoutColor = UWHDPixel(  0,   0, 255);
const UWHDPixel UWHDTimeoutColor      = UWHDPixel(255, 255,   0);
const UWHDPixel UWHDHalfTimeColor     = UWHDPixel(255, 160,   0);
const UWHDPixel UWHDGameOverColor     = UWHDPixel(255,   0,   0);

#define _ 0
#define NW 0x1
#define SW 0x2
#define NE 0x4
#define SE 0x8

#define TL SW|NW|NE
#define BL NW|SW|SE
#define BR SW|SE|NE
#define TR SE|NE|NW

#define LT NW|SW
#define RT NE|SE
#define TP NW|NE
#define BT SW|SE

static char HalfTime[32*32] = {
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
_, _, _, 1, 1, _, _, 1, 1, _, _, 1, 1, 1, 1, _, _, 1, 1, _, _, _, _, _, 1, 1, 1, 1, 1, _, _, _,
_, _, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, _, _, _, 1, 1, 1, 1, 1, _, _, _,
_, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, _, _, 1, 1, _, _, _, _, _, _,
_, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, _, _, 1, 1, _, _, _, _, _, _,
_, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, _, _, _, 1, 1, 1, 1, 1, _, _, _,
_, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, _, _, _, 1, 1, 1, 1, 1, _, _, _,
_, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, _, _, 1, 1, _, _, _, _, _, _,
_, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, _, _, 1, 1, _, _, _, _, _, _,
_, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, _, _, _, _,
_, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
_, _, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, _, _,
_, 1, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, 1, _,
1, 1, 1, 1, 1, 1, 1, 1, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, 1, 1, 1, 1, 1, 1, 1, 1,
_, 1, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, 1, _,
_, _, 1, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, 1, _, _,
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
_, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, _, _, _,
_, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, _, _, _,
_, _, _, _, _, 1, 1, _, _, _, _, _, 1, 1, _, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, 1, 1, _, _, _, _, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, 1, 1, _, _, _, _, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, _, _, _,
_, _, _, _, _, 1, 1, _, _, _, _, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, _, _, _,
_, _, _, _, _, 1, 1, _, _, _, _, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, 1, 1, _, _, _, _, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, 1, 1, _, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, _, _, _,
_, _, _, _, _, 1, 1, _, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, _, _, _,
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
};

static char GameOver[32*32] = {
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, 1, 1, 1, 1, 1, _, _, 1, 1, 1, 1, _, _, 1, 1, _, _, 1, 1, _, _, 1, 1, 1, 1, 1, _, _,
_, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, 1, _, _,
_, _, _, 1, 1, _, _, _, _, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, _, _, _, _,
_, _, _, 1, 1, _, _, _, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, _, _, _,
_, _, _, 1, 1, _, 1, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, 1, _, _,
_, _, _, 1, 1, _, 1, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, 1, _, _,
_, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, _, _, _,
_, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, _, _, _,
_, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, 1, _, _,
_, _, _, _, 1, 1, 1, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, _, 1, 1, 1, 1, 1, _, _,
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
_, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, 1, 1, _, _, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, _, _, _,
_, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, 1, _, _,
_, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, _, _, 1, 1, _, _, 1, 1, _, _,
_, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, _, _, 1, 1, _, _, 1, 1, _, _,
_, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, _, _, _,
_, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, _, _, _, _,
_, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, _, _, 1, 1, _, 1, 1, _, _, _,
_, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, _, _, 1, 1, _, _, 1, _, _, _,
_, _, _, 1, 1, 1, 1, 1, 1, _, _, 1, 1, 1, 1, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, 1, 1, _, _,
_, _, _, 1, 1, 1, 1, 1, 1, _, _, _, 1, 1, _, _, _, _, 1, 1, 1, 1, 1, _, 1, 1, _, _, 1, 1, _, _,
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _,
};

static char Ref[32*10] = {
_, _, _, _, _, _, 1, 1, 1, 1, 1, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, _, _, 1, 1, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, _, _, 1, 1, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, _, _, _, 1, 1, 1, 1, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, _, 1, 1, 1, 1, 1, _, _, 1, 1, 1, 1, _, _, _, 1, 1, 1, 1, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, _, _, _, _, 1, 1, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, _, _, 1, 1, _, 1, 1, _, _, 1, 1, _, _, _, _, _, 1, 1, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, _, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, _, _, _, _, _, _, _, _,
_, _, _, _, _, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, _, _, _, _, _, _, _, _,
};

static char Timeout[32*10] = {
_, _, _, _, _, _, 1, 1, 1, 1, 1, 1, _, _, _, _, _, _, _, _, 1, 1, 1, 1, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, _, 1, 1, 1, 1, 1, 1, _, _, _, _, _, 1, 1, _, 1, 1, 1, 1, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, 1, 1, _, _, _, _, _, _, 1, 1, _, _, 1, 1, _, _, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, 1, 1, _, _, _, _, _, _, 1, 1, _, _, 1, 1, _, _, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, 1, 1, _, _, _, _, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, 1, 1, _, _, _, _, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, 1, 1, _, _, _, _, 1, 1, _, _, _, _, 1, 1, _, _, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, 1, 1, _, _, _, _, 1, 1, _, _, _, _, 1, 1, _, _, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, 1, 1, _, _, _, 1, 1, _, _, _, _, _, 1, 1, 1, 1, 1, 1, _, _, _, _, _, _,
_, _, _, _, _, _, _, _, 1, 1, _, _, _, _, _, _, _, _, _, _, 1, 1, 1, 1, 1, 1, _, _, _, _, _, _,
};

static char Black[32*10] = {
_, 1, 1, 1, 1, _, _, _, 1, 1, _, _, _, _, 1, 1, 1, 1, _, _, 1, 1, 1, 1, _, 1, 1, _, _, 1, 1, _,
_, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, _, 1, 1, _, _, 1, _, _,
_, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, 1, 1, _, 1, 1, _, _,
_, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, 1, 1, 1, 1, _, _, _,
_, 1, 1, 1, 1, 1, _, _, 1, 1, _, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, _, 1, 1, 1, _, _, _, _,
_, 1, 1, 1, 1, 1, _, _, 1, 1, _, _, _, 1, 1, 1, 1, 1, 1, _, 1, 1, _, _, _, 1, 1, 1, _, _, _, _,
_, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, 1, 1, 1, 1, _, _, _,
_, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, _, _, 1, 1, _, 1, 1, _, _,
_, 1, 1, 1, 1, 1, 1, _, 1, 1, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, _, 1, 1, _, 1, 1, _, _,
_, 1, 1, 1, 1, 1, _, _, 1, 1, 1, 1, _, 1, 1, _, _, 1, 1, _, 1, 1, 1, 1, _, 1, 1, _, _, 1, 1, _,
};

static char White[32*10] = {
_, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, 1, 1, _, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, _,
_, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, 1, 1, _, 1, 1, 1, 1, _, 1, 1, 1, 1, 1, _,
_, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, _, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, _, _, _, _,
_, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, _, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, _, _, _, _,
_, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, 1, 1, 1, _, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, 1, 1, _, _,
_, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, 1, 1, 1, _, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, 1, 1, _, _,
_, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, _, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, _, _, _, _,
_, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, _, _, 1, 1, _, _, _, 1, 1, _, _, 1, 1, _, _, _, _,
_, 1, 1, 1, 1, 1, 1, 1, 1, _, 1, 1, _, 1, 1, _, 1, 1, 1, 1, _, _, 1, 1, _, _, 1, 1, 1, 1, 1, _,
_, _, 1, 1, _, _, 1, 1, _, _, 1, 1, _, 1, 1, _, 1, 1, 1, 1, _, _, 1, 1, _, _, 1, 1, 1, 1, 1, _,
};


#undef _

static void renderColon(UWHDCanvas *C, unsigned X, unsigned Y) {
  C->at(    X,     Y) = UWHDColonColor;
  C->at(    X, 1 + Y) = UWHDColonColor;
  C->at(1 + X,     Y) = UWHDColonColor;
  C->at(1 + X, 1 + Y) = UWHDColonColor;
}

static void renderCondensedTime(UWHDCanvas *C, unsigned DisplayNum,
                                unsigned Now, const UWHDPixel &FG,
                                const UWHDPixel *BG) {
  // We can't yet display larger times than 99:59
  if (99 * 60 + 59 < Now)
    Now = 99 * 60 + 59;

  // Note that we show 1h30m as 90m here:
  unsigned Mins = (Now / 60) % 100;
  unsigned MTens = Mins / 10;
  unsigned MOnes = Mins % 10;

  unsigned Secs = Now % 60;
  unsigned STens = Secs / 10;
  unsigned SOnes = Secs % 10;

  unsigned xoffs = DisplayNum * 32;

  if (10 <= Mins) {
    // Minutes Ten's
    BigNumber::Render(C, 1, MTens, /*xo=*/5, /*yo=*/3,
                      BigNumber::Font::Digit11x20, FG, BG);

    // Minutes One's
    BigNumber::Render(C, 1, MOnes, /*xo=*/18, /*yo=*/3,
                      BigNumber::Font::Digit11x20, FG, BG);

    // Seconds Ten's
    BigNumber::Render(C, 1, STens, /*xo=*/12, /*yo=*/24,
                      BigNumber::Font::Digit5x7, FG, BG);

    // Seconds One's
    BigNumber::Render(C, 1, SOnes, /*xo=*/19, /*yo=*/24,
                      BigNumber::Font::Digit5x7, FG, BG);

    // Colons
    C->at(xoffs + 10, 26) = FG;
    C->at(xoffs + 10, 28) = FG;
  } else if (1 <= Mins && Mins < 10) {
    // Minutes One's
    BigNumber::Render(C, 1, MOnes, /*xo=*/12, /*yo=*/1,
                      BigNumber::Font::Digit11x20, FG, BG);

    // Seconds Ten's
    BigNumber::Render(C, 1, STens, /*xo=*/12, /*yo=*/24,
                      BigNumber::Font::Digit5x7, FG, BG);

    // Seconds One's
    BigNumber::Render(C, 1, SOnes, /*xo=*/19, /*yo=*/24,
                      BigNumber::Font::Digit5x7, FG, BG);

    // Colons
    C->at(xoffs + 10, 26) = FG;
    C->at(xoffs + 10, 28) = FG;
  } else {
    // Seconds Ten's
    BigNumber::Render(C, 1, STens, /*xo=*/6, /*yo=*/7,
                      BigNumber::Font::Digit11x20, FG, BG);

    // Seconds One's
    BigNumber::Render(C, 1, SOnes, /*xo=*/19, /*yo=*/7,
                      BigNumber::Font::Digit11x20, FG, BG);

    // Top Colon
    C->at(xoffs + 2, 13) = FG;
    C->at(xoffs + 3, 13) = FG;
    C->at(xoffs + 2, 14) = FG;
    C->at(xoffs + 3, 14) = FG;

    // Bottom Colon
    C->at(xoffs + 2, 19) = FG;
    C->at(xoffs + 3, 19) = FG;
    C->at(xoffs + 2, 20) = FG;
    C->at(xoffs + 3, 20) = FG;
  }
}

void renderWallClock(UWHDCanvas *C) {
  time_t WallClock = time(nullptr);
  struct tm *LocalClock;
  LocalClock = localtime(&WallClock);
  unsigned HH = LocalClock->tm_hour;
  unsigned MM = LocalClock->tm_min;
  unsigned SS = LocalClock->tm_sec;

  BigNumber::Render(C, 0, HH / 10, /*xo=*/ 3,  /*yo=*/ 2,
                    BigNumber::Font::Digit11x20, UWHDSecondsColor,
                    &UWHDBackground);
  BigNumber::Render(C, 0, HH % 10, /*xo=*/ 18, /*yo=*/ 2,
                    BigNumber::Font::Digit11x20, UWHDSecondsColor,
                    &UWHDBackground);

  renderColon(C, 31, 7);
  renderColon(C, 31, 15);

  BigNumber::Render(C, 1, MM / 10, /*xo=*/ 3,  /*yo=*/ 2,
                    BigNumber::Font::Digit11x20, UWHDSecondsColor,
                    &UWHDBackground);
  BigNumber::Render(C, 1, MM % 10, /*xo=*/ 18, /*yo=*/ 2,
                    BigNumber::Font::Digit11x20, UWHDSecondsColor,
                    &UWHDBackground);

  renderColon(C, 63, 7);
  renderColon(C, 63, 15);

  BigNumber::Render(C, 2, SS / 10, /*xo=*/ 3,  /*yo=*/ 2,
                    BigNumber::Font::Digit11x20, UWHDSecondsColor,
                    &UWHDBackground);
  BigNumber::Render(C, 2, SS % 10, /*xo=*/ 18, /*yo=*/ 2,
                    BigNumber::Font::Digit11x20, UWHDSecondsColor,
                    &UWHDBackground);

  if ((SS / 10) % 2 == 0)
    BigNumber::printf(C, 24, 24, UWHDLogoColor2, nullptr, "NAVISJON");
  else
    BigNumber::printf(C, 21, 24, UWHDLogoColor1, nullptr, "TIMESHARK");
}

void renderHalfTime(unsigned Now, bool Toggle, UWHDCanvas *C) {
  unsigned xoffs = 32;
  if (Toggle) {
    renderCondensedTime(C, 1, Now, UWHDHalfTimeColor, &UWHDBackground);
  } else {
    for (int y = 0; y < 32; y++)
      for (int x = 0; x < 32; x++)
        if (HalfTime[x + y * 32])
          C->at(xoffs + x, y) = UWHDTimeoutColor;
  }
}

void renderGameOver(unsigned Now, bool Toggle, UWHDCanvas *C) {
  unsigned xoffs = 32;
  if (Toggle) {
    renderCondensedTime(C, 1, Now, UWHDGameOverColor, &UWHDBackground);
  } else {
    for (int y = 0; y < 32; y++)
      for (int x = 0; x < 32; x++)
        if (GameOver[x + y * 32])
          C->at(xoffs + x, y) = UWHDTimeoutColor;
  }
}

void renderTimeoutRing(UWHDCanvas *C) {
  unsigned xoffs = 32;

  // Draw a ring around the center display for emphasis that this is a time out:
  for (int x = 0; x < 32; x++) {
    C->at(xoffs + x, 0) = UWHDTimeoutColor;
    C->at(xoffs + x, 31) = UWHDTimeoutColor;
  }
  for (int y = 1; y < 31; ++y) {
    C->at(xoffs,      y) = UWHDTimeoutColor;
    C->at(xoffs  +31, y) = UWHDTimeoutColor;
  }
}

void renderRefTimeout(unsigned Now, bool Toggle, UWHDCanvas *C) {
  unsigned xoffs = 32;
  if (Toggle) {
    renderCondensedTime(C, 1, Now, UWHDTimeoutColor, &UWHDBackground);
  } else {
    for (int y = 0; y < 10; y++)
      for (int x = 0; x < 32; x++)
        if (Ref[x + y * 32])
          C->at(xoffs + x, 2 + y) = UWHDTimeoutColor;

    for (int y = 0; y < 10; y++)
      for (int x = 0; x < 32; x++)
        if (Timeout[x + y * 32])
          C->at(xoffs + x, 20 + y) = UWHDTimeoutColor;
  }
  renderTimeoutRing(C);
}

void renderBlackTimeout(unsigned Now, bool Toggle, UWHDCanvas *C) {
  unsigned xoffs = 32;
  if (Toggle) {
    renderCondensedTime(C, 1, Now, UWHDTimeoutColor, &UWHDBackground);
  } else {
    for (int y = 0; y < 10; y++)
      for (int x = 0; x < 32; x++)
        if (Black[x + y * 32])
          C->at(xoffs + x, 3 + y) = UWHDBlackTimeoutColor;

    for (int y = 0; y < 10; y++)
      for (int x = 0; x < 32; x++)
        if (Black[x + y * 32])
          C->at(xoffs + x, 19 + y) = UWHDBlackTimeoutColor;
  }
}

void renderWhiteTimeout(unsigned Now, bool Toggle, UWHDCanvas *C) {
  unsigned xoffs = 32;
  if (Toggle) {
    renderCondensedTime(C, 1, Now, UWHDTimeoutColor, &UWHDBackground);
  } else {
    for (int y = 0; y < 10; y++)
      for (int x = 0; x < 32; x++)
        if (White[x + y * 32])
          C->at(xoffs + x, 3 + y) = UWHDWhiteTimeoutColor;

    for (int y = 0; y < 10; y++)
      for (int x = 0; x < 32; x++)
        if (White[x + y * 32])
          C->at(xoffs + x, 19 + y) = UWHDWhiteTimeoutColor;
  }
}

void renderTimeDisplay(GameModel M, UWHDCanvas *C) {
  unsigned Now = M.GameClockSecs;

  // We can't yet display larger times than 99:59
  if (99 * 60 + 59 < Now)
    Now = 99 * 60 + 59;

  unsigned xoffs = 32;

  // Alternate based on the wall clock instead of the game clock,
  // as the game clock might be paused.
  bool Toggle = time(nullptr) % 4;

  if (M.TS != GameModel::TS_RefTimeout &&
      M.TS != GameModel::TS_WhiteTimeout &&
      M.TS != GameModel::TS_BlackTimeout) {
    switch (M.GS) {
    case GameModel::GS_WallClock:
      renderWallClock(C);
      break;
    case GameModel::GS_HalfTime:
      renderHalfTime(Now, Toggle, C);
      break;
    case GameModel::GS_GameOver:
      renderGameOver(Now, Toggle, C);
      break;
    case GameModel::GS_FirstHalf:
    case GameModel::GS_SecondHalf:
      break;
    }
  }

  if (M.GS != GameModel::GS_WallClock) {
    switch (M.TS) {
    case GameModel::TS_None:
      renderCondensedTime(C, 1, Now, UWHDSecondsColor, &UWHDBackground);
      break;
    case GameModel::TS_RefTimeout:
      renderRefTimeout(Now, Toggle, C);
      break;
    case GameModel::TS_WhiteTimeout:
      renderWhiteTimeout(Now, Toggle, C);
      break;
    case GameModel::TS_BlackTimeout:
      renderBlackTimeout(Now, Toggle, C);
      break;
    }
  }
}
