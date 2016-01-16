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
using namespace uwhtimer;

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

static char TimeOver[256] =
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

void TimeDisplay::Render(Canvas *C) {
  GameModel M = Mgr.getModel();
  unsigned Now = M.GameClockSecs;

  unsigned xoffs = DisplayNum * 32;

  if (M.GameClockSecs != 0) {
    // Note that we show 1h30m as 90m here:
    unsigned Mins = (Now / 60) % 100;
    unsigned MTens = Mins / 10;
    unsigned MOnes = Mins % 10;

    // Minutes
    if (MTens) {
      BigNumber::RenderHalfSingle(C, DisplayNum, MTens, 1, 2, MinutesColor, &Background);
      BigNumber::RenderHalfSingle(C, DisplayNum, MOnes, 15, 2, MinutesColor, &Background);
    } else {
      BigNumber::RenderHalfSingle(C, DisplayNum, MOnes, 8, 2, MinutesColor, &Background);
    }

    unsigned Secs = Now % 60;
    unsigned STens = Secs / 10;
    unsigned SOnes = Secs % 10;

    // Seconds
    BigNumber::RenderQuarterSingle(C, DisplayNum, STens, 8, 20, SecondsColor, &Background);
    BigNumber::RenderQuarterSingle(C, DisplayNum, SOnes, 15, 20, SecondsColor, &Background);

    // Top Colon
    C->SetPixel(xoffs + 6, 22, ColonColor.r, ColonColor.g, ColonColor.b);
    C->SetPixel(xoffs + 7, 22, ColonColor.r, ColonColor.g, ColonColor.b);
    C->SetPixel(xoffs + 6, 23, ColonColor.r, ColonColor.g, ColonColor.b);
    C->SetPixel(xoffs + 7, 23, ColonColor.r, ColonColor.g, ColonColor.b);

    // Bottom Colon
    C->SetPixel(xoffs + 6, 25, ColonColor.r, ColonColor.g, ColonColor.b);
    C->SetPixel(xoffs + 7, 25, ColonColor.r, ColonColor.g, ColonColor.b);
    C->SetPixel(xoffs + 6, 26, ColonColor.r, ColonColor.g, ColonColor.b);
    C->SetPixel(xoffs + 7, 26, ColonColor.r, ColonColor.g, ColonColor.b);
  } else {
    Color StopColor(255, 0, 0);
    for (unsigned y = 0; y < 16; y++) {
      for (unsigned x = 0; x < 16; x++) {
        if (TimeOver[x + y * 16] & NW) {
          C->SetPixel(xoffs + x * 2 + 0, y * 2 + 0, StopColor.r, StopColor.g, StopColor.b);
        }

        if (TimeOver[x + y * 16] & SW) {
          C->SetPixel(xoffs + x * 2 + 0, y * 2 + 1, StopColor.r, StopColor.g, StopColor.b);
        }

        if (TimeOver[x + y * 16] & NE) {
          C->SetPixel(xoffs + x * 2 + 1, y * 2 + 0, StopColor.r, StopColor.g, StopColor.b);
        }

        if (TimeOver[x + y * 16] & SE) {
          C->SetPixel(xoffs + x * 2 + 1, y * 2 + 1, StopColor.r, StopColor.g, StopColor.b);
        }
      }
    }
  }

  SecondsRing::Render(C, DisplayNum, 120 - (Now % 120), RingColor, &Background);

}
