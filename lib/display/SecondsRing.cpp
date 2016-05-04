//===-- SecondsRing.cpp - Renders a Snake-like Seconds Hand -------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/display/SecondsRing.h"

#include <canvas.h>
#include <led-matrix.h>
#include <graphics.h>

#include <algorithm>
#include <cassert>
#include <stdio.h>

using namespace rgb_matrix;

void SecondsRing::WriteSegment(Canvas *Canvas,
                               unsigned x,
                               unsigned y,
                               const Color &FG,
                               const Color *BG,
                               bool Growing,
                               bool IsTail) {
  if (Growing) {
    if (IsTail)
      Canvas->SetPixel(x, y, FG.r, FG.g, FG.b);
    else {
      if (BG)
        Canvas->SetPixel(x, y, BG->r, BG->g, BG->b);
    }
  } else {
    if (IsTail) {
      if (BG)
        Canvas->SetPixel(x, y, BG->r, BG->g, BG->b);
    } else
      Canvas->SetPixel(x, y, FG.r, FG.g, FG.b);
  }
}

void SecondsRing::Render(Canvas *Canvas,
                         unsigned Display,
                         unsigned Seconds,
                         const Color &FG,
                         const Color *BG) {
  unsigned xoffs = Display * 32;

  bool Growing = (Seconds % 120) >= 60;
  Seconds = Seconds % 60;

  int Dial = ((Seconds + 1) * 30 * 4) / 60;

  // Top Right
  for (int x = 16; x <= 30; x++)
    WriteSegment(Canvas, xoffs + x, 0, FG, BG, Growing,
                 (Dial = std::max(0, Dial-1)));

  // Right Side
  for (int y = 1; y <= 30; y++)
    WriteSegment(Canvas, xoffs + 31, y, FG, BG, Growing,
                 (Dial = std::max(0, Dial-1)));

  // Bottom
  for (int x = 30; x >= 1; x--)
    WriteSegment(Canvas, xoffs + x, 31, FG, BG, Growing,
                 (Dial = std::max(0, Dial-1)));

  // Left Side
  for (int y = 30; y >= 1; y--)
    WriteSegment(Canvas, xoffs + 0, y, FG, BG, Growing,
                 (Dial = std::max(0, Dial-1)));

  // Top Left
  for (int x = 1; x < 16; x++)
    WriteSegment(Canvas, xoffs + x, 0, FG, BG, Growing,
                 (Dial = std::max(0, Dial-1)));

  assert(Dial == 0);
}

