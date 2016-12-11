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
    , GS(GameModel::GS_WallClock)
    , TS(GameModel::TS_None)
    , MK(GameModel::MK_Master) {}
  unsigned char BlackScore;
  unsigned char WhiteScore;
  int GameClockSecs;         // Amount of time left in the game when wall clock == PrevStartTime
  struct timeval PrevStartTime;
  bool ClockRunning;

  enum GameState {
    GS_WallClock,
    GS_FirstHalf,
    GS_SecondHalf,
    GS_HalfTime,
    GS_GameOver,
  };
  GameState GS;

  enum TimeoutState {
    TS_None,
    TS_RefTimeout,
    TS_WhiteTimeout,
    TS_BlackTimeout
  };
  TimeoutState TS;

  enum ModelKind {
    MK_Master,
    MK_PassiveSlave,
    MK_ActiveSlave,
  };
  ModelKind MK;

  std::string dump() const;
  std::string serialize() const;
  static bool deSerialize(std::string S, GameModel &M);

  void setPrevStartTime();
  int displayedTimeLeft() const;

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
  int gameClock();
  bool gameClockRunning();

  GameModel::GameState gameState();
  bool gameStateWallClock();
  bool gameStateFirstHalf();
  bool gameStateSecondHalf();
  bool gameStateHalfTime();
  bool gameStateGameOver();

  GameModel::TimeoutState timeoutState();
  bool timeoutStateNone();
  bool timeoutStateRef();
  bool timeoutStateBlack();
  bool timeoutStateWhite();

  GameModel::ModelKind modelKind();
  bool modelKindMaster();
  bool modelKindPassiveSlave();
  bool modelKindActiveSlave();

  void setBlackScore(unsigned char S);
  void setWhiteScore(unsigned char S);
  void setGameClock(int T);
  void setGameClockRunning(bool B);

  void setGameState(GameModel::GameState GS);
  void setGameStateWallClock();
  void setGameStateFirstHalf();
  void setGameStateSecondHalf();
  void setGameStateHalfTime();
  void setGameStateGameOver();

  void setTimeoutState(GameModel::TimeoutState TS);
  void setTimeoutStateNone();
  void setTimeoutStateRef();
  void setTimeoutStateBlack();
  void setTimeoutStateWhite();

  void setModelKind(GameModel::ModelKind K);
  void setModelKindMaster();
  void setModelKindPassiveSlave();
  void setModelKindActiveSlave();

  virtual void heartbeat();

private:
  std::mutex ModelMutex;
  GameModel Model;
  std::vector<GameModelListener*> Listeners;
  static const int HeartbeatDelayMs;
};

#endif
