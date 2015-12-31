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

namespace uwhtimer {

class GameModel {
public:
  unsigned BlackScore;
  unsigned WhiteScore;
  unsigned GameClockSecs;
};

class GameModelManager {
public:
  GameModelManager() {}

  GameModel *getModel();

private:
  GameModel Model;
};

}

#endif
