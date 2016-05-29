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

class GameModel {
public:
  GameModel()
    : BlackScore(0)
    , WhiteScore(0)
    , GameClockSecs(0)
    , State(GameModel::WallClock) {}
  unsigned char BlackScore;
  unsigned char WhiteScore;
  unsigned short GameClockSecs;
  bool ClockRunning;

  enum GameState {
    WallClock,
    NormalPlay,
    HalfTime,
    RefTimeOut,
    TeamTimeOut,
    GameOver,
  };
  GameState State;

  std::string dump() const;
  std::string serialize() const;
  static bool deSerialize(std::string S, GameModel &M);

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
  bool gameStateNormalPlay();
  bool gameStateHalfTime();
  bool gameStateRefTimeOut();
  bool gameStateTeamTimeOut();
  bool gameStateGameOver();

  void setBlackScore(unsigned char S);
  void setWhiteScore(unsigned char S);
  void setGameClock(unsigned short T);
  void setGameClockRunning(bool B);

  void setGameState(GameModel::GameState S);
  void setGameStateWallClock();
  void setGameStateNormalPlay();
  void setGameStateHalfTime();
  void setGameStateRefTimeOut();
  void setGameStateTeamTimeOut();
  void setGameStateGameOver();

  void heartbeat();

private:
  std::mutex ModelMutex;
  GameModel Model;
  std::vector<GameModelListener*> Listeners;
  static const int HeartbeatDelayMs;
};

#endif
