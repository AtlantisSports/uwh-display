//===-- GameModel.cpp - Model for Game State ----------------------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/model/GameModel.h"

#include <ctime>
#include <iomanip>
#include <string>
#include <sstream>
#include <future>
#include <cstdio>

using namespace uwhtimer;

bool GameModel::operator==(const GameModel &Other) {
  return BlackScore == Other.BlackScore &&
         WhiteScore == Other.WhiteScore &&
         GameClockSecs == Other.GameClockSecs;
}

std::string GameModel::dump() {
  std::stringstream SS;

  SS << "Black: " << int(BlackScore) << "\n"
     << "White: " << int(WhiteScore) << "\n"
     << "GameClockSecs: " << int(GameClockSecs) << "\n";

  return SS.str();
}

std::string GameModel::serialize() {
  std::stringstream SS;

  SS << "S"
     << "B" << int(BlackScore)
     << "W" << int(WhiteScore)
     << "T" << int(GameClockSecs)
     << "E";

  return SS.str();
}

static bool check(std::stringstream &SS, char V) {
  char C;
  SS.get(C);
  return C != V;
}

bool GameModel::deSerialize(std::string S, GameModel &M) {
  GameModel NewM;
  std::stringstream SS;
  SS << S;

  if (check(SS, 'S'))
    return true;

  if (check(SS, 'B'))
    return true;

  {
    int B;
    SS >> B;
    NewM.BlackScore = B;
  }

  if (check(SS, 'W'))
    return true;

  {
    int W;
    SS >> W;
    NewM.WhiteScore = W;
  }

  if (check(SS, 'T'))
    return true;

  {
    int T;
    SS >> T;
    NewM.GameClockSecs = T;
  }

  if (check(SS, 'E'))
    return true;

  M = NewM;
  return false;
}

const int GameModelManager::HeartbeatDelayMs = 10;

GameModelManager::GameModelManager()
    : Model()
    , ClockRunning(false) {
  std::thread([this]() {
    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(HeartbeatDelayMs));
      Heartbeat();
    }
  }).detach();
}

GameModel GameModelManager::getModel() {
  return Model;
}

bool GameModelManager::toggleGameClock() {
  return ClockRunning = !ClockRunning;
}

void GameModelManager::Heartbeat() {
  static time_t PrevTime = time(nullptr);
  time_t Now = time(nullptr);

  GameModel Cur = getModel();
  if (ClockRunning) {
    double Delta = difftime(Now, PrevTime);
    if (Delta >= 1)
      Cur.GameClockSecs -= int(Delta);
    else
      return;
  }
  setModel(Cur);

  PrevTime = Now;
}
