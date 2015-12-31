//===-- ColorSettings.cpp - Stylistic Setup for Colors ------------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/display/GameDisplay.h"
#include "uwhd/display/TimeDisplay.h"

#include <graphics.h>

using namespace rgb_matrix;
using namespace uwhtimer;

const Color GameDisplay::Background   = Color(  0,   0,   0);
const Color GameDisplay::WhiteTeamFG  = Color(255, 255, 255);
const Color GameDisplay::BlackTeamFG  = Color(  0,   0, 255);
const Color GameDisplay::WhiteTeamBG  = Background;
const Color GameDisplay::BlackTeamBG  = Background;

const Color TimeDisplay::SecondsColor = Color(  0, 255,   0);
const Color TimeDisplay::MinutesColor = Color(  0, 255,   0);
const Color TimeDisplay::ColonColor   = Color(  0, 255,   0);
const Color TimeDisplay::RingColor    = Color(255, 255,   0);
const Color TimeDisplay::Background   = GameDisplay::Background;

