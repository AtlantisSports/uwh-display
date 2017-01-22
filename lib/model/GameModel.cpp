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
         ClockRunning == Other.ClockRunning &&
         ((ClockRunning && MK != MK_PassiveSlave) ||
          GameClockSecs == Other.GameClockSecs) &&
         GS == Other.GS &&
         TS == Other.TS;
}

std::string GameModel::dump() const {
  std::stringstream SS;

  SS << "        Black: " << int(BlackScore) << "\n"
     << "        White: " << int(WhiteScore) << "\n"
     << "GameClockSecs: " << int(GameClockSecs) << "\n"
     << " ClockRunning: " << (ClockRunning ? "YES" : "NO") << "\n"
     << "    GameState: ";

  switch (GS) {
  case GameModel::GS_WallClock:    SS << "Wall Clock"; break;
  case GameModel::GS_FirstHalf:    SS << "First Half"; break;
  case GameModel::GS_SecondHalf:   SS << "Second Half"; break;
  case GameModel::GS_HalfTime:     SS << "Half Time"; break;
  case GameModel::GS_GameOver:     SS << "Game Over"; break;
  default:                         SS << "???"; break;
  }

  SS << "\n"
     << "      Timeout: ";

  switch (TS) {
  case GameModel::TS_None:         SS << "None"; break;
  case GameModel::TS_RefTimeout:   SS << "Ref Timeout"; break;
  case GameModel::TS_BlackTimeout: SS << "Black Timeout"; break;
  case GameModel::TS_WhiteTimeout: SS << "White Timeout"; break;
  default:                         SS << "???"; break;
  }

  SS << "\n"
     << "         Kind: ";

  switch (MK) {
  case GameModel::MK_Master:       SS << "Master"; break;
  case GameModel::MK_ActiveSlave:  SS << "Active Slave"; break;
  case GameModel::MK_PassiveSlave: SS << "Passive Slave"; break;
  default:                         SS << "???"; break;
  }

  SS << "\n";

  return SS.str();
}

std::string GameModel::serialize() const {
  std::stringstream SS;

  SS << "S"
     << "B" << int(BlackScore)
     << "W" << int(WhiteScore)
     << "T" << int(displayedTimeLeft())
     << "R" << (ClockRunning ? 1 : 0)
     << 'G';

  switch (GS) {
  case GameModel::GS_WallClock:    SS << 'C'; break;
  case GameModel::GS_FirstHalf:    SS << 'F'; break;
  case GameModel::GS_SecondHalf:   SS << 'S'; break;
  case GameModel::GS_HalfTime:     SS << 'H'; break;
  case GameModel::GS_GameOver:     SS << 'O'; break;
  default:                         SS << '?'; break;
  }

  SS << "P"; // 'P' as in Pause

  switch (TS) {
  case GameModel::TS_None:         SS << 'N'; break;
  case GameModel::TS_RefTimeout:   SS << 'R'; break;
  case GameModel::TS_WhiteTimeout: SS << 'W'; break;
  case GameModel::TS_BlackTimeout: SS << 'B'; break;
  default:                         SS << '?'; break;
  }

#if 0
  // Don't serialize the kind, it needs to be set independently
  // based on where it's being used.
  SS << 'K';

  switch (MK) {
  case GameModel::MK_Master:       SS << "M"; break;
  case GameModel::MK_ActiveSlave:  SS << "A"; break;
  case GameModel::MK_PassiveSlave: SS << "P"; break;
  default:                         SS << "?"; break;
  }
#endif

  SS << "E";

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

  if (check(SS, 'G'))
    return true;

  char GS;
  SS.get(GS);
  switch (GS) {
  case 'C': NewM.GS = GameModel::GS_WallClock; break;
  case 'F': NewM.GS = GameModel::GS_FirstHalf; break;
  case 'S': NewM.GS = GameModel::GS_SecondHalf; break;
  case 'H': NewM.GS = GameModel::GS_HalfTime; break;
  case 'O': NewM.GS = GameModel::GS_GameOver; break;
  default: return true;
  }

  if (check(SS, 'P'))
    return true;

  char TS;
  SS.get(TS);
  switch (TS) {
  case 'N': NewM.TS = GameModel::TS_None; break;
  case 'R': NewM.TS = GameModel::TS_RefTimeout; break;
  case 'W': NewM.TS = GameModel::TS_WhiteTimeout; break;
  case 'B': NewM.TS = GameModel::TS_BlackTimeout; break;
  default: return true;
  }

#if 0
  // Don't serialize the kind, it needs to be set independently based on where
  // it is being used.
  if (check(SS, 'K'))
    return true;

  char MK;
  SS.get(MK);
  switch (MK) {
  case 'M': NewM.MK = GameModel::MK_Master; break;
  case 'A': NewM.MK = GameModel::MK_ActiveSlave; break;
  case 'P': NewM.MK = GameModel::MK_PassiveSlave; break;
  default: return true;
  }
#endif

  if (check(SS, 'E'))
    return true;

  gettimeofday(&NewM.PrevStartTime, nullptr);

  Mod = NewM;
  return false;
}

static long timevaldiff(const struct timeval *Start,
                        const struct timeval *End) {
  long msec = (End->tv_sec - Start->tv_sec) * 1000;
  msec += (End->tv_usec - Start->tv_usec) / 1000;
  return msec;
}

void GameModel::setPrevStartTime() {
  gettimeofday(&PrevStartTime, nullptr);
}

int GameModel::displayedTimeLeft() const {
  struct timeval Now;
  gettimeofday(&Now, nullptr);

  if (ClockRunning) {
    long msec = timevaldiff(&PrevStartTime, &Now);
    return GameClockSecs - msec / 1000;
  } else
    return GameClockSecs;
}

const int GameModelManager::HeartbeatDelayMs = 100;

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

int GameModelManager::gameClock() {
  std::lock_guard<std::mutex> Lock(ModelMutex);

  switch (Model.MK) {
  case GameModel::MK_Master:
  case GameModel::MK_ActiveSlave:
    return Model.displayedTimeLeft();
  case GameModel::MK_PassiveSlave:
  default:
    return Model.GameClockSecs;
  }
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

void GameModelManager::setGameClock(int T) {
  GameModel M;
  {
    std::lock_guard<std::mutex> Lock(ModelMutex);
    Model.GameClockSecs = T;
    gettimeofday(&Model.PrevStartTime, nullptr);
    if (T == 0)
      Model.ClockRunning = false;
    M = Model;
  }
  modelChanged(M);
}

void GameModelManager::setGameClockRunning(bool B) {
  GameModel M;
  {
    std::lock_guard<std::mutex> Lock(ModelMutex);

    if (!Model.ClockRunning && B)
      gettimeofday(&Model.PrevStartTime, nullptr);

    Model.ClockRunning = B;

    M = Model;
  }
  modelChanged(M);
}

GameModel::GameState GameModelManager::gameState() {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model.GS;
}

bool GameModelManager::gameStateWallClock() {
  return gameState() == GameModel::GS_WallClock;
}

bool GameModelManager::gameStateFirstHalf() {
  return gameState() == GameModel::GS_FirstHalf;
}

bool GameModelManager::gameStateSecondHalf() {
  return gameState() == GameModel::GS_SecondHalf;
}

bool GameModelManager::gameStateHalfTime() {
  return gameState() == GameModel::GS_HalfTime;
}

bool GameModelManager::gameStateGameOver() {
  return gameState() == GameModel::GS_GameOver;
}

GameModel::TimeoutState GameModelManager::timeoutState() {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model.TS;
}

bool GameModelManager::timeoutStateNone() {
  return timeoutState() == GameModel::TS_None;
}

bool GameModelManager::timeoutStateRef() {
  return timeoutState() == GameModel::TS_RefTimeout;
}

bool GameModelManager::timeoutStateWhite() {
  return timeoutState() == GameModel::TS_WhiteTimeout;
}

bool GameModelManager::timeoutStateBlack() {
  return timeoutState() == GameModel::TS_BlackTimeout;
}

GameModel::ModelKind GameModelManager::modelKind() {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model.MK;
}

bool GameModelManager::modelKindMaster() {
  return modelKind() == GameModel::MK_Master;
}

bool GameModelManager::modelKindPassiveSlave() {
  return modelKind() == GameModel::MK_PassiveSlave;
}

bool GameModelManager::modelKindActiveSlave() {
  return modelKind() == GameModel::MK_ActiveSlave;
}

void GameModelManager::setGameState(GameModel::GameState GS) {
  GameModel M;
  {
    std::lock_guard<std::mutex> Lock(ModelMutex);
    Model.GS = GS;
    M = Model;
  }
  modelChanged(M);
}

void GameModelManager::setTimeoutState(GameModel::TimeoutState TS) {
  GameModel M;
  {
    std::lock_guard<std::mutex> Lock(ModelMutex);
    Model.TS = TS;
    M = Model;
  }
  modelChanged(M);
}

void GameModelManager::setGameStateWallClock() {
  setGameState(GameModel::GS_WallClock);
}

void GameModelManager::setGameStateFirstHalf() {
  setGameState(GameModel::GS_FirstHalf);
}

void GameModelManager::setGameStateSecondHalf() {
  setGameState(GameModel::GS_SecondHalf);
}

void GameModelManager::setGameStateHalfTime() {
  setGameState(GameModel::GS_HalfTime);
}

void GameModelManager::setGameStateGameOver() {
  setGameState(GameModel::GS_GameOver);
}

void GameModelManager::setTimeoutStateNone() {
  setTimeoutState(GameModel::TS_None);
}

void GameModelManager::setTimeoutStateRef() {
  setTimeoutState(GameModel::TS_RefTimeout);
}

void GameModelManager::setTimeoutStateWhite() {
  setTimeoutState(GameModel::TS_WhiteTimeout);
}

void GameModelManager::setTimeoutStateBlack() {
  setTimeoutState(GameModel::TS_BlackTimeout);
}

void GameModelManager::setModelKindMaster() {
  setModelKind(GameModel::MK_Master);
}

void GameModelManager::setModelKindPassiveSlave() {
  setModelKind(GameModel::MK_PassiveSlave);
}

void GameModelManager::setModelKindActiveSlave() {
  setModelKind(GameModel::MK_ActiveSlave);
}

void GameModelManager::setModelKind(GameModel::ModelKind MK) {
  GameModel M;
  {
    std::lock_guard<std::mutex> Lock(ModelMutex);
    Model.MK = MK;
    M = Model;
  }
  modelChanged(M);
}

void GameModelManager::heartbeat() {
  modelChanged(Model);
}

void GameModelManager::modelChanged(GameModel M) {
  for (auto *L : Listeners)
    L->modelChanged(M);
}

void GameModelManager::registerListener(GameModelListener *L) {
  Listeners.push_back(L);
}
