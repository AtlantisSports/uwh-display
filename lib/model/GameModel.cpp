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
#include <iostream>

bool GameModel::operator==(const GameModel &Other) const {
  return BlackScore == Other.BlackScore &&
         WhiteScore == Other.WhiteScore &&
         GameClockSecs == Other.GameClockSecs &&
         ClockRunning == Other.ClockRunning;
}

std::string GameModel::dump() const {
  std::stringstream SS;

  SS << "        Black: " << int(BlackScore) << "\n"
     << "        White: " << int(WhiteScore) << "\n"
     << "GameClockSecs: " << int(GameClockSecs) << "\n"
     << " ClockRunning: " << (ClockRunning ? "YES" : "NO") << "\n";

  return SS.str();
}

std::string GameModel::serialize() const {
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

bool GameModel::deSerialize(std::string S, GameModel &Mod) {
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

  Mod = NewM;
  return false;
}

const int GameModelManager::HeartbeatDelayMs = 10;

GameModelManager::GameModelManager()
    : Model() {
  std::thread([this]() {
    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(HeartbeatDelayMs));
      heartbeat();
    }
  }).detach();
}

void GameModelManager::setModel(GameModel Mod) {
  GameModel M;
  {
    std::lock_guard<std::mutex> Lock(ModelMutex);
    Model = Mod;
    M = Model;
  }
  modelChanged(M);
}

GameModel GameModelManager::getModel() {
  GameModel M;
  {
    std::lock_guard<std::mutex> Lock(ModelMutex);
    M = Model;
  }
  return M;
}

unsigned char GameModelManager::blackScore() {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model.BlackScore;
}

unsigned char GameModelManager::whiteScore() {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model.WhiteScore;
}

unsigned short GameModelManager::gameClock() {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model.GameClockSecs;
}

bool GameModelManager::gameClockRunning() {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model.ClockRunning;
}

void GameModelManager::setBlackScore(unsigned char S) {
  GameModel M;
  {
    std::lock_guard<std::mutex> Lock(ModelMutex);
    Model.BlackScore = S;
    M = Model;
  }
  modelChanged(M);
}

void GameModelManager::setWhiteScore(unsigned char S) {
  GameModel M;
  {
    std::lock_guard<std::mutex> Lock(ModelMutex);
    Model.WhiteScore = S;
    M = Model;
  }
  modelChanged(M);
}

void GameModelManager::setGameClock(unsigned short T) {
  GameModel M;
  {
    std::lock_guard<std::mutex> Lock(ModelMutex);
    Model.GameClockSecs = T;
    M = Model;
  }
  modelChanged(M);
}

void GameModelManager::setGameClockRunning(bool B) {
  GameModel M;
  {
    std::lock_guard<std::mutex> Lock(ModelMutex);
    Model.ClockRunning = B;
    M = Model;
  }
  modelChanged(M);
}

void GameModelManager::heartbeat() {
  static time_t PrevTime = time(nullptr);
  time_t Now = time(nullptr);

  if (gameClockRunning()) {
    double Delta = difftime(Now, PrevTime);
    if (Delta >= 1) {
      setGameClock(gameClock() - 1);
    } else
      return;
  }

  PrevTime = Now;
}

void GameModelManager::modelChanged(GameModel M) {
  for (auto *L : Listeners)
    L->modelChanged(M);
}

void GameModelManager::registerListener(GameModelListener *L) {
  Listeners.push_back(L);
}
