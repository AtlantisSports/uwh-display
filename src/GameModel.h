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
