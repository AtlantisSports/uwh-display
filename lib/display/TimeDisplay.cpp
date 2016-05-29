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
#include "uwhd/display/SecondsRing.h"
#include "uwhd/model/GameModel.h"

#include <graphics.h>

#include <ctime>

using namespace rgb_matrix;

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

static char TimeUp[256] =
{   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _, 0xF, 0xF, 0xF,   _, 0xF,   _, 0xF,  BL,  BR, 0xF,   _, 0xF, 0xF, 0xF,   _,
    _,   _, 0xF,   _,   _, 0xF,   _, 0xF,  TR,  TL, 0xF,   _, 0xF,   _,   _,   _,
    _,   _, 0xF,   _,   _, 0xF,   _, 0xF,   _,   _, 0xF,   _, 0xF, 0xF,   _,   _,
    _,   _, 0xF,   _,   _, 0xF,   _, 0xF,   _,   _, 0xF,   _, 0xF,   _,   _,   _,
    _,   _, 0xF,   _,   _, 0xF,   _, 0xF,   _,   _, 0xF,   _, 0xF, 0xF, 0xF,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _, 0xF,   _, 0xF,   _, 0xF, 0xF, 0xF,   _,   _,   _,   _,
    _,   _,   _,   _,   _, 0xF,   _, 0xF,   _, 0xF,   _, 0xF,   _,   _,   _,   _,
    _,   _,   _,   _,   _, 0xF,   _, 0xF,   _, 0xF, 0xF, 0xF,   _,   _,   _,   _,
    _,   _,   _,   _,   _, 0xF,   _, 0xF,   _, 0xF,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _, 0xF, 0xF, 0xF,   _, 0xF,   _,   _,   _,   _,   _,   _,
    _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _,   _ };
#undef _

static void renderColon(TimeDisplay &T, Canvas *C, unsigned X, unsigned Y) {
  C->SetPixel(    X,     Y, T.ColonColor.r, T.ColonColor.g, T.ColonColor.b);
  C->SetPixel(    X, 1 + Y, T.ColonColor.r, T.ColonColor.g, T.ColonColor.b);
  C->SetPixel(1 + X,     Y, T.ColonColor.r, T.ColonColor.g, T.ColonColor.b);
  C->SetPixel(1 + X, 1 + Y, T.ColonColor.r, T.ColonColor.g, T.ColonColor.b);
}

void TimeDisplay::Render(Canvas *C) {
  GameModel M = Mgr.getModel();

  unsigned Now = M.GameClockSecs;
  time_t WallClock = time(nullptr);
  struct tm *LocalClock;
  LocalClock = localtime(&WallClock);
  unsigned HH = LocalClock->tm_hour;
  unsigned MM = LocalClock->tm_min;
  unsigned SS = LocalClock->tm_sec;

  unsigned xoffs = DisplayNum * 32;

  if (M.State == GameModel::WallClock) {
    BigNumber::Render(C, 0, HH / 10, /*xo=*/ 3,  /*yo=*/ 2, BigNumber::Font::Digit11x20, SecondsColor, &Background);
    BigNumber::Render(C, 0, HH % 10, /*xo=*/ 18, /*yo=*/ 2, BigNumber::Font::Digit11x20, SecondsColor, &Background);

    renderColon(*this, C, 31, 7);
    renderColon(*this, C, 31, 15);

    BigNumber::Render(C, 1, MM / 10, /*xo=*/ 3,  /*yo=*/ 2, BigNumber::Font::Digit11x20, SecondsColor, &Background);
    BigNumber::Render(C, 1, MM % 10, /*xo=*/ 18, /*yo=*/ 2, BigNumber::Font::Digit11x20, SecondsColor, &Background);

    renderColon(*this, C, 63, 7);
    renderColon(*this, C, 63, 15);

    BigNumber::Render(C, 2, SS / 10, /*xo=*/ 3,  /*yo=*/ 2, BigNumber::Font::Digit11x20, SecondsColor, &Background);
    BigNumber::Render(C, 2, SS % 10, /*xo=*/ 18, /*yo=*/ 2, BigNumber::Font::Digit11x20, SecondsColor, &Background);

    BigNumber::printf(C, 21, 24, LogoColor, nullptr, "TIMESHARK");
  } else {
    if (M.GameClockSecs != 0) {
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

      if (10 <= Mins) {
        // Minutes Ten's
        BigNumber::Render(C, 1, MTens, /*xo=*/5, /*yo=*/3,
                          BigNumber::Font::Digit11x20, MinutesColor, &Background);

        // Minutes One's
        BigNumber::Render(C, 1, MOnes, /*xo=*/18, /*yo=*/3,
                          BigNumber::Font::Digit11x20, MinutesColor, &Background);

        // Seconds Ten's
        BigNumber::Render(C, 1, STens, /*xo=*/12, /*yo=*/24,
                          BigNumber::Font::Digit5x7, SecondsColor, &Background);

        // Seconds One's
        BigNumber::Render(C, 1, SOnes, /*xo=*/19, /*yo=*/24,
                          BigNumber::Font::Digit5x7, SecondsColor, &Background);

        // Colons
        C->SetPixel(xoffs + 10, 26, ColonColor.r, ColonColor.g, ColonColor.b);
        C->SetPixel(xoffs + 10, 28, ColonColor.r, ColonColor.g, ColonColor.b);
      } else if (1 <= Mins && Mins < 10) {
        // Minutes One's
        BigNumber::Render(C, 1, MOnes, /*xo=*/12, /*yo=*/1,
                          BigNumber::Font::Digit11x20, MinutesColor, &Background);

        // Seconds Ten's
        BigNumber::Render(C, 1, STens, /*xo=*/12, /*yo=*/24,
                          BigNumber::Font::Digit5x7, SecondsColor, &Background);

        // Seconds One's
        BigNumber::Render(C, 1, SOnes, /*xo=*/19, /*yo=*/24,
                          BigNumber::Font::Digit5x7, SecondsColor, &Background);

        // Colons
        C->SetPixel(xoffs + 10, 26, ColonColor.r, ColonColor.g, ColonColor.b);
        C->SetPixel(xoffs + 10, 28, ColonColor.r, ColonColor.g, ColonColor.b);
      } else {
        // Seconds Ten's
        BigNumber::Render(C, 1, STens, /*xo=*/6, /*yo=*/7,
                          BigNumber::Font::Digit11x20, SecondsColor, &Background);

        // Seconds One's
        BigNumber::Render(C, 1, SOnes, /*xo=*/19, /*yo=*/7,
                          BigNumber::Font::Digit11x20, SecondsColor, &Background);

        // Top Colon
        C->SetPixel(xoffs + 2, 13, ColonColor.r, ColonColor.g, ColonColor.b);
        C->SetPixel(xoffs + 3, 13, ColonColor.r, ColonColor.g, ColonColor.b);
        C->SetPixel(xoffs + 2, 14, ColonColor.r, ColonColor.g, ColonColor.b);
        C->SetPixel(xoffs + 3, 14, ColonColor.r, ColonColor.g, ColonColor.b);

        // Bottom Colon
        C->SetPixel(xoffs + 2, 19, ColonColor.r, ColonColor.g, ColonColor.b);
        C->SetPixel(xoffs + 3, 19, ColonColor.r, ColonColor.g, ColonColor.b);
        C->SetPixel(xoffs + 2, 20, ColonColor.r, ColonColor.g, ColonColor.b);
        C->SetPixel(xoffs + 3, 20, ColonColor.r, ColonColor.g, ColonColor.b);
      }
    } else {
      Color StopColor(255, 0, 0);
      for (unsigned y = 0; y < 16; y++) {
        for (unsigned x = 0; x < 16; x++) {
          if (TimeUp[x + y * 16] & NW) {
            C->SetPixel(xoffs + x * 2 + 0, y * 2 + 0, StopColor.r, StopColor.g, StopColor.b);
          }

          if (TimeUp[x + y * 16] & SW) {
            C->SetPixel(xoffs + x * 2 + 0, y * 2 + 1, StopColor.r, StopColor.g, StopColor.b);
          }

          if (TimeUp[x + y * 16] & NE) {
            C->SetPixel(xoffs + x * 2 + 1, y * 2 + 0, StopColor.r, StopColor.g, StopColor.b);
          }

          if (TimeUp[x + y * 16] & SE) {
            C->SetPixel(xoffs + x * 2 + 1, y * 2 + 1, StopColor.r, StopColor.g, StopColor.b);
          }
        }
      }
    }

    SecondsRing::Render(C, DisplayNum, 120 - (Now % 120), RingColor, &Background);
  }
}
