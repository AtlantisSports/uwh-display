//===-- GameModel.cpp - Model for Game State ----------------------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/GameModel.h"

#include <ctime>

using namespace uwhtimer;

GameModel *GameModelManager::getModel() {
  Model.BlackScore = 1;
  Model.WhiteScore = 3;
  Model.GameClockSecs = 60 * 12 - (time(nullptr) % (60 * 12));
  return &Model;
}
