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

class GameModel {
public:
  GameModel()
    : BlackScore(0)
    , WhiteScore(0)
    , GameClockSecs(0) {}
  unsigned char BlackScore;
  unsigned char WhiteScore;
  unsigned short GameClockSecs;
  bool ClockRunning;

  std::string dump();
  std::string serialize();
  static bool deSerialize(std::string S, GameModel &M);

  bool operator==(const GameModel &Other);
  bool operator!=(const GameModel &Other) { return !(*this == Other); }
};

class GameModelManager {
public:
  GameModelManager();

  GameModel getModel();
  void setModel(GameModel M);
  virtual void modelChanged(GameModel M) {}

  unsigned char BlackScore();
  unsigned char WhiteScore();
  unsigned short GameClockSecs();
  bool ClockRunning();

  void setBlackScore(unsigned char S);
  void setWhiteScore(unsigned char S);
  void setGameClockSecs(unsigned short T);
  void setClockRunning(bool B);

  unsigned char incBlackScore(signed char Delta);
  unsigned char incWhiteScore(signed char Delta);
  unsigned short incGameClock(signed short Delta);
  bool toggleClockRunning();

  void Heartbeat();

private:
  std::mutex ModelMutex;
  GameModel Model;
  static const int HeartbeatDelayMs;
};

#endif
