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

TimeDisplay::TimeDisplay(unsigned DisplayNum)
  : DisplayNum(DisplayNum) {}

void TimeDisplay::Render(Canvas *C) {
  Color Yellow(255, 255, 0);
  Color Green(0, 255, 0);
  Color Black(0, 0, 0);

  unsigned Now = time(nullptr);
  unsigned Secs = Now % 60;
  unsigned Tens = Secs / 10;
  unsigned Ones = Secs % 10;

  // Minutes
  BigNumber::RenderHalfSingle(C, DisplayNum, Tens, 1, 2, Green, &Black);
  BigNumber::RenderHalfSingle(C, DisplayNum, Ones, 15, 2, Green, &Black);

  // Seconds
  BigNumber::RenderQuarterSingle(C, DisplayNum, Tens, 8, 20, Green, &Black);
  BigNumber::RenderQuarterSingle(C, DisplayNum, Ones, 15, 20, Green, &Black);

  // Top Colon
  C->SetPixel(6, 22, Green.r, Green.g, Green.b);
  C->SetPixel(7, 22, Green.r, Green.g, Green.b);
  C->SetPixel(6, 23, Green.r, Green.g, Green.b);
  C->SetPixel(7, 23, Green.r, Green.g, Green.b);

  // Bottom Colon
  C->SetPixel(6, 25, Green.r, Green.g, Green.b);
  C->SetPixel(7, 25, Green.r, Green.g, Green.b);
  C->SetPixel(6, 26, Green.r, Green.g, Green.b);
  C->SetPixel(7, 26, Green.r, Green.g, Green.b);

  SecondsRing::Render(C, DisplayNum, Now, Yellow, &Black);
}

#endif
