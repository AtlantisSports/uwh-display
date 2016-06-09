//===-- GameModel.h - Model for Game State ------------------------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//


#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <string>
#include <mutex>
#include <vector>
#include <sys/time.h>

class GameModel {
public:
  GameModel()
    : BlackScore(0)
    , WhiteScore(0)
    , GameClockSecs(0)
    , State(GameModel::WallClock) {}
  unsigned char BlackScore;
  unsigned char WhiteScore;
  unsigned GameClockSecs;         // Amount of time left in the game when wall clock == PrevStartTime
  struct timeval PrevStartTime;
  bool ClockRunning;

  enum GameState {
    WallClock,
    FirstHalf,
    SecondHalf,
    HalfTime,
    RefTimeOut,
    WhiteTimeOut,
    BlackTimeOut,
    GameOver,
  };
  GameState State;

  std::string dump() const;
  std::string serialize() const;
  static bool deSerialize(std::string S, GameModel &M);

  void setPrevStartTime();
  unsigned displayedTimeLeft();

  bool operator==(const GameModel &Other) const;
  bool operator!=(const GameModel &Other) const { return !(*this == Other); }
};

struct GameModelListener {
  virtual void modelChanged(GameModel M) {}
  virtual ~GameModelListener() {}
};

class GameModelManager : public GameModelListener {
public:
  GameModelManager();
  virtual ~GameModelManager() {}

  void registerListener(GameModelListener *L);

  GameModel getModel();
  void setModel(GameModel M);
  virtual void modelChanged(GameModel M);

  unsigned char blackScore();
  unsigned char whiteScore();
  unsigned short gameClock();
  bool gameClockRunning();
  GameModel::GameState gameState();
  bool gameStateWallClock();
  bool gameStateFirstHalf();
  bool gameStateSecondHalf();
  bool gameStateHalfTime();
  bool gameStateRefTimeOut();
  bool gameStateWhiteTimeOut();
  bool gameStateBlackTimeOut();
  bool gameStateGameOver();

  void setBlackScore(unsigned char S);
  void setWhiteScore(unsigned char S);
  void setGameClock(unsigned short T);
  void setGameClockRunning(bool B);

  void setGameState(GameModel::GameState S);
  void setGameStateWallClock();
  void setGameStateFirstHalf();
  void setGameStateSecondHalf();
  void setGameStateHalfTime();
  void setGameStateRefTimeOut();
  void setGameStateBlackTimeOut();
  void setGameStateWhiteTimeOut();
  void setGameStateGameOver();

  virtual void heartbeat();

private:
  std::mutex ModelMutex;
  GameModel Model;
  std::vector<GameModelListener*> Listeners;
  static const int HeartbeatDelayMs;
};

#endif
