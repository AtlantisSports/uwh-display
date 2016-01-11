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
#include <thread>

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
     << "R" << (ClockRunning ? 1 : 0)
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

  if (check(SS, 'R'))
    return true;

  {
    int R;
    SS >> R;
    NewM.ClockRunning = (R == 1);
  }

  if (check(SS, 'E'))
    return true;

  M = NewM;
  return false;
}

const int GameModelManager::HeartbeatDelayMs = 10;

GameModelManager::GameModelManager()
    : Model() {
  std::thread([this]() {
    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(HeartbeatDelayMs));
      Heartbeat();
    }
  }).detach();
}

unsigned char GameModelManager::BlackScore() {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model.BlackScore;
}

unsigned char GameModelManager::WhiteScore() {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model.WhiteScore;
}

unsigned short GameModelManager::GameClockSecs() {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model.GameClockSecs;
}

bool GameModelManager::ClockRunning() {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model.ClockRunning;
}

GameModel GameModelManager::getModel() {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model;
}

void GameModelManager::setBlackScore(unsigned char S) {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  Model.BlackScore = S;
}

void GameModelManager::setWhiteScore(unsigned char S) {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  Model.WhiteScore = S;
}

void GameModelManager::setGameClock(unsigned short T) {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  Model.GameClockSecs = T;
}

void GameModelManager::setClockRunning(bool B) {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  Model.ClockRunning = B;
}

unsigned char GameModelManager::incBlackScore(signed char Delta) {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model.BlackScore += Delta;
}

unsigned char GameModelManager::incWhiteScore(signed char Delta) {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model.WhiteScore += Delta;
}

unsigned short GameModelManager::incGameClock(signed short Delta) {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model.GameClockSecs += Delta;
}

bool GameModelManager::toggleClockRunning() {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model.ClockRunning = !Model.ClockRunning;
}

void GameModelManager::Heartbeat() {
  static time_t PrevTime = time(nullptr);
  time_t Now = time(nullptr);

  if (ClockRunning()) {
    double Delta = difftime(Now, PrevTime);
    if (Delta >= 1)
      incGameClock(-int(Delta));
    else
      return;
  }

  PrevTime = Now;
}
