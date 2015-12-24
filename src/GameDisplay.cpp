#include "GameDisplay.h"

#include "BigNumber.h"

#include <graphics.h>
#include <led-matrix.h>

using namespace rgb_matrix;
using namespace uwhtimer;

void GameDisplay::Run() {
  Color Blue(0, 0, 255);
  Color White(200, 200, 200);
  Color Black(0, 0, 0);
  FrameCanvas *Frame = Mtx->CreateFrameCanvas();
  while (running()) {
    GameModel *Model = Mgr.getModel();
    BigNumber::Render(Frame, 0, Model->BlackScore, Blue, &Black);
    TD.Render(Frame);
    BigNumber::Render(Frame, 2, Model->WhiteScore, White, &Black);
    Frame = Mtx->SwapOnVSync(Frame);
  }
}
