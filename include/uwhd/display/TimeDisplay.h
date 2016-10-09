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

class GameModel;
class GameModelManager;

class TimeDisplay : public RenderElement {
public:
    TimeDisplay(unsigned DisplayNum, GameModelManager &Mgr)
      : DisplayNum(DisplayNum)
      , Mgr(Mgr) {}

    void Render(rgb_matrix::Canvas *C);
private:
    unsigned DisplayNum;
    GameModelManager &Mgr;

public:
    static const rgb_matrix::Color SecondsColor;
    static const rgb_matrix::Color MinutesColor;
    static const rgb_matrix::Color ColonColor;
    static const rgb_matrix::Color RingColor;
    static const rgb_matrix::Color Background;
    static const rgb_matrix::Color LogoColor1;
    static const rgb_matrix::Color LogoColor2;
    static const rgb_matrix::Color WhiteTimeOutColor;
    static const rgb_matrix::Color BlackTimeOutColor;
    static const rgb_matrix::Color TimeOutColor;
    static const rgb_matrix::Color HalfTimeColor;
    static const rgb_matrix::Color GameOverColor;
};

#endif

