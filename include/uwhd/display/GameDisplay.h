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

class UWHDCanvas;

void renderGameDisplay(GameModel M, UWHDCanvas *C);

#endif
