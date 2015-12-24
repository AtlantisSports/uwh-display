#include "GameModel.h"

#include <ctime>

using namespace uwhtimer;

GameModel *GameModelManager::getModel() {
  Model.BlackScore = 1;
  Model.WhiteScore = 3;
  Model.GameClockSecs = time(nullptr);
  return &Model;
}
