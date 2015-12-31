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

#include "RenderElement.h"

#include <graphics.h>

namespace rgb_matrix {
class Canvas;
}

namespace uwhtimer {
class GameModel;
class GameModelManager;

class TimeDisplay : public RenderElement {
public:
    TimeDisplay(unsigned DisplayNum, GameModelManager &Mgr)
      : DisplayNum(DisplayNum)
      , Mgr(Mgr) {}

    void Render(rgb_matrix::Canvas *C) override;
private:
    unsigned DisplayNum;
    GameModelManager &Mgr;

public:
    static const rgb_matrix::Color SecondsColor;
    static const rgb_matrix::Color MinutesColor;
    static const rgb_matrix::Color ColonColor;
    static const rgb_matrix::Color RingColor;
    static const rgb_matrix::Color Background;
};

}

#endif

