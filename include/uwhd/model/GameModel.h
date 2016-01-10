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

namespace uwhtimer {

class GameModel {
public:
  GameModel()
    : BlackScore(0)
    , WhiteScore(0)
    , GameClockSecs(0) {}
  unsigned char BlackScore;
  unsigned char WhiteScore;
  unsigned short GameClockSecs;

  std::string dump();
  std::string serialize();
  static bool deSerialize(std::string S, GameModel &M);

  bool operator==(const GameModel &Other);
  bool operator!=(const GameModel &Other) { return !(*this == Other); }
};

class GameModelManager {
public:
  GameModelManager() {}

  GameModel *getModel();

  virtual void setModel(GameModel M) { Model = M; }

private:
  GameModel Model;
};

}

#endif
