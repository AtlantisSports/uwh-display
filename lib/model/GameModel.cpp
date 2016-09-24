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
         ((ClockRunning && Kind != PassiveSlave) ||
          GameClockSecs == Other.GameClockSecs) &&
         State == Other.State;
}

std::string GameModel::dump() const {
  std::stringstream SS;

  SS << "        Black: " << int(BlackScore) << "\n"
     << "        White: " << int(WhiteScore) << "\n"
     << "GameClockSecs: " << int(GameClockSecs) << "\n"
     << " ClockRunning: " << (ClockRunning ? "YES" : "NO") << "\n"
     << "        State: ";

  switch (State) {
  case GameModel::WallClock:    SS << "Wall Clock"; break;
  case GameModel::RawData:      SS << "Raw Data"; break;
  case GameModel::FirstHalf:    SS << "First Half"; break;
  case GameModel::SecondHalf:   SS << "Second Half"; break;
  case GameModel::HalfTime:     SS << "Half Time"; break;
  case GameModel::RefTimeOut:   SS << "Ref Timeout"; break;
  case GameModel::WhiteTimeOut: SS << "White Timeout"; break;
  case GameModel::BlackTimeOut: SS << "Black Timeout"; break;
  case GameModel::GameOver:     SS << "Game Over"; break;
  default:                      SS << "???"; break;
  }

  SS << "\n"
     << "         Kind: ";

  switch (State) {
  case GameModel::Master:       SS << "Master"; break;
  case GameModel::ActiveSlave:  SS << "Active Slave"; break;
  case GameModel::PassiveSlave: SS << "Passive Slave"; break;
  default:                      SS << "???"; break;
  }

  SS << "\n";

  return SS.str();
}

std::string GameModel::serialize() const {
  std::stringstream SS;

  if (State == GameState::RawData) {
    SS << "<<";

    for (int y = 0; y < 32; ++y)
      for (int x = 0; x < 32 * 3; ++x)
        SS << Data[y * 32 * 3 + x];

    SS << ">>";
    return SS.str();
  }

  SS << "S"
     << "B" << int(BlackScore)
     << "W" << int(WhiteScore)
     << "T" << int(displayedTimeLeft())
     << "R" << (ClockRunning ? 1 : 0)
     << 'G';

  switch (State) {
  case GameModel::WallClock:    SS << 'C'; break;
  case GameModel::FirstHalf:    SS << 'F'; break;
  case GameModel::SecondHalf:   SS << 'S'; break;
  case GameModel::HalfTime:     SS << 'H'; break;
  case GameModel::RefTimeOut:   SS << 'R'; break;
  case GameModel::WhiteTimeOut: SS << 'W'; break;
  case GameModel::BlackTimeOut: SS << 'B'; break;
  case GameModel::GameOver:     SS << 'O'; break;
  default:                      SS << '?'; break;
  }

#if 0
  // Don't serialize the kind, it needs to be set independently
  // based on where it's being used.
  SS << 'K';

  switch (State) {
  case GameModel::Master:       SS << "M"; break;
  case GameModel::ActiveSlave:  SS << "A"; break;
  case GameModel::PassiveSlave: SS << "P"; break;
  default:                      SS << "?"; break;
  }
#endif

  SS << "E";

  return SS.str();
}

bool GameModel::deSerialize(std::string S, GameModel &Mod) {
  GameModel NewM;
  std::stringstream SS;
  SS << S;

  char C;
  SS.get(C);
  if (C == '<') {
    printf("deserialize raw:\n");

    SS.get(C);
    if (C != '<')
      return true;

    for (int y = 0; y < 32; ++y)
      for (int x = 0; x < 32 * 3; ++x) {
        char CC;
        SS.get(CC);
        NewM.Data[y * 32 * 3 + x] = CC;
        printf("%c", CC);
    }

    SS.get(C);
    if (C != '>')
      return true;

    SS.get(C);
    if (C != '>')
      return true;

    printf("\nsuccess\n");

    NewM.State = GameModel::RawData;

    Mod = NewM;
    return false;
  }

  if (C != 'S')
    return true;

  SS.get(C);
  if (C != 'B')
    return true;

  {
    int B;
    SS >> B;
    NewM.BlackScore = B;
  }

  SS.get(C);
  if (C != 'W')
    return true;

  {
    int W;
    SS >> W;
    NewM.WhiteScore = W;
  }

  SS.get(C);
  if (C != 'T')
    return true;

  {
    int T;
    SS >> T;
    NewM.GameClockSecs = T;
  }

  SS.get(C);
  if (C != 'R')
    return true;

  {
    int R;
    SS >> R;
    NewM.ClockRunning = (R == 1);
  }

  SS.get(C);
  if (C != 'G')
    return true;

  char GS;
  SS.get(GS);
  switch (GS) {
  case 'C': NewM.State = GameModel::WallClock; break;
  case 'F': NewM.State = GameModel::FirstHalf; break;
  case 'S': NewM.State = GameModel::SecondHalf; break;
  case 'H': NewM.State = GameModel::HalfTime; break;
  case 'R': NewM.State = GameModel::RefTimeOut; break;
  case 'W': NewM.State = GameModel::WhiteTimeOut; break;
  case 'B': NewM.State = GameModel::BlackTimeOut; break;
  case 'O': NewM.State = GameModel::GameOver; break;
  default: return true;
  }

#if 0
  // Don't serialize the kind, it needs to be set independently based on where
  // it is being used.
  SS.get(C);
  if (C != 'K')
    return true;

  char MK;
  SS.get(MK);
  switch (MK) {
  case 'M': NewM.Kind = GameModel::Master; break;
  case 'A': NewM.Kind = GameModel::ActiveSlave; break;
  case 'P': NewM.Kind = GameModel::PassiveSlave; break;
  default: return true;
  }
#endif

  SS.get(C);
  if (C != 'E')
    return true;

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

  switch (Model.Kind) {
  case GameModel::Master:
  case GameModel::ActiveSlave:
    return Model.displayedTimeLeft();
  case GameModel::PassiveSlave:
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
  return Model.State;
}


bool GameModelManager::gameStateWallClock() {
  return gameState() == GameModel::WallClock;
}

bool GameModelManager::gameStateFirstHalf() {
  return gameState() == GameModel::FirstHalf;
}

bool GameModelManager::gameStateSecondHalf() {
  return gameState() == GameModel::SecondHalf;
}

bool GameModelManager::gameStateHalfTime() {
  return gameState() == GameModel::HalfTime;
}

bool GameModelManager::gameStateRefTimeOut() {
  return gameState() == GameModel::RefTimeOut;
}

bool GameModelManager::gameStateWhiteTimeOut() {
  return gameState() == GameModel::WhiteTimeOut;
}

bool GameModelManager::gameStateBlackTimeOut() {
  return gameState() == GameModel::BlackTimeOut;
}

bool GameModelManager::gameStateGameOver() {
  return gameState() == GameModel::GameOver;
}

GameModel::ModelKind GameModelManager::modelKind() {
  std::lock_guard<std::mutex> Lock(ModelMutex);
  return Model.Kind;
}

bool GameModelManager::modelKindMaster() {
  return modelKind() == GameModel::Master;
}

bool GameModelManager::modelKindPassiveSlave() {
  return modelKind() == GameModel::PassiveSlave;
}

bool GameModelManager::modelKindActiveSlave() {
  return modelKind() == GameModel::ActiveSlave;
}

void GameModelManager::setGameState(GameModel::GameState S) {
  GameModel M;
  {
    std::lock_guard<std::mutex> Lock(ModelMutex);
    Model.State = S;
    M = Model;
  }
  modelChanged(M);
}

void GameModelManager::setGameStateWallClock() {
  setGameState(GameModel::WallClock);
}

void GameModelManager::setGameStateFirstHalf() {
  setGameState(GameModel::FirstHalf);
}

void GameModelManager::setGameStateSecondHalf() {
  setGameState(GameModel::SecondHalf);
}

void GameModelManager::setGameStateHalfTime() {
  setGameState(GameModel::HalfTime);
}

void GameModelManager::setGameStateRefTimeOut() {
  setGameState(GameModel::RefTimeOut);
}

void GameModelManager::setGameStateWhiteTimeOut() {
  setGameState(GameModel::WhiteTimeOut);
}

void GameModelManager::setGameStateBlackTimeOut() {
  setGameState(GameModel::BlackTimeOut);
}

void GameModelManager::setGameStateGameOver() {
  setGameState(GameModel::GameOver);
}

void GameModelManager::setModelKindMaster() {
  setModelKind(GameModel::Master);
}

void GameModelManager::setModelKindPassiveSlave() {
  setModelKind(GameModel::PassiveSlave);
}

void GameModelManager::setModelKindActiveSlave() {
  setModelKind(GameModel::ActiveSlave);
}

void GameModelManager::setModelKind(GameModel::ModelKind K) {
  GameModel M;
  {
    std::lock_guard<std::mutex> Lock(ModelMutex);
    Model.Kind = K;
    M = Model;
  }
  modelChanged(M);
}

void GameModelManager::heartbeat() { }

void GameModelManager::modelChanged(GameModel M) {
  for (auto *L : Listeners)
    L->modelChanged(M);
}

void GameModelManager::registerListener(GameModelListener *L) {
  Listeners.push_back(L);
}
