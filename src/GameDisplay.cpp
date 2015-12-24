#include "GameDisplay.h"

#include "BigNumber.h"

#include <graphics.h>
#include <led-matrix.h>

using namespace rgb_matrix;
using namespace uwhtimer;

void GameDisplay::Run() {
  FrameCanvas *Frame = Mtx->CreateFrameCanvas();
  while (running()) {
    GameModel *Model = Mgr.getModel();
    BigNumber::Render(Frame, 0, Model->BlackScore, BlackTeamFG, &BlackTeamBG);
    TD.Render(Frame);
    BigNumber::Render(Frame, 2, Model->WhiteScore, WhiteTeamFG, &WhiteTeamBG);
    Frame = Mtx->SwapOnVSync(Frame);
  }
}
