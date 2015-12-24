#ifndef TIMEDISPLAY_H
#define TIMEDISPLAY_H

#include "TimeDisplay.h"

#include "SecondsRing.h"

#include <graphics.h>
#include <gpio.h>
#include <led-matrix.h>
#include <threaded-canvas-manipulator.h>

#include <ctime>

using namespace rgb_matrix;
using namespace uwhtimer;

TimeDisplay::TimeDisplay(RGBMatrix *M, unsigned DisplayNum)
  : ThreadedCanvasManipulator(M)
  , M(M)
  , DisplayNum(DisplayNum) {}

void TimeDisplay::Run() {
  Color Yellow(255, 255, 0);
  Color Green(0, 255, 0);
  Color Black(0, 0, 0);
  FrameCanvas *Frame = M->CreateFrameCanvas();
  while (running()) {
    unsigned Now = time(nullptr);
    unsigned Secs = Now % 60;
    unsigned Tens = Secs / 10;
    unsigned Ones = Secs % 10;

    // Minutes
    BigNumber::RenderHalfSingle(Frame, DisplayNum, Tens, 1, 2, Green, &Black);
    BigNumber::RenderHalfSingle(Frame, DisplayNum, Ones, 15, 2, Green, &Black);

    // Seconds
    BigNumber::RenderQuarterSingle(Frame, DisplayNum, Tens, 8, 18, Green, &Black);
    BigNumber::RenderQuarterSingle(Frame, DisplayNum, Ones, 15, 18, Green, &Black);

    SecondsRing::Render(Frame, DisplayNum, Secs, Yellow, &Black);
    Frame = M->SwapOnVSync(Frame);
  }
}

#endif
