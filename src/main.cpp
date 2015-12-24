#include "BigNumber.h"
#include "GameModel.h"
#include "SecondsRing.h"
#include "TimeDisplay.h"

#include "graphics.h"
#include "gpio.h"
#include "led-matrix.h"
#include "threaded-canvas-manipulator.h"

#include <algorithm>
#include <cinttypes>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <ctime>

using namespace rgb_matrix;
using namespace uwhtimer;

class GameDisplay : public ThreadedCanvasManipulator {
public:
  GameDisplay(RGBMatrix *Mtx)
    : ThreadedCanvasManipulator(Mtx)
    , Mtx(Mtx)
    , Mgr()
    , TD(1, Mgr)
  {}

  void Run() {
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
private:
  RGBMatrix *Mtx;
  GameModelManager Mgr;
  TimeDisplay TD;
};

int main(int argc, char *argv[]) {
  GPIO IO;

  if (!IO.Init()) {
      std::cerr << "Error: Could not init GPIO. Try again with 'sudo'?\n";
      exit(-1);
  }

  auto Matrix = std::unique_ptr<RGBMatrix>(new RGBMatrix(&IO, 32, 3, 1));
  Matrix->SetPWMBits(4);

  auto Display = std::unique_ptr<GameDisplay>(new GameDisplay(&*Matrix));
  Display->Start();

  sleep(INT_MAX);
}
