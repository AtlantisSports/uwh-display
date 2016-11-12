//===-- TimeDisplay.h - Renders a 32x32 Time Panel ----------------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//


#ifndef TIMEDISPLAY_H
#define TIMEDISPLAY_H

#include "uwhd/canvas/Canvas.h"

class GameModel;

void renderTimeDisplay(GameModel M, UWHDCanvas *C);

#endif

