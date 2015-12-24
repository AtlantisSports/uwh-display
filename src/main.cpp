#include "BigNumber.h"
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

class GameModel {
public:
    unsigned BlackScore;
    unsigned WhiteScore;
    unsigned GameClockMins;
    unsigned GameClockSecs;
};

class GameDisplay : public ThreadedCanvasManipulator {
public:
    GameDisplay(RGBMatrix *M, GameModel *Model)
        : ThreadedCanvasManipulator(M)
        , M(M)
        , TD(0) {}

    void Run() {
      Font F;
      if (!F.LoadFont("matrix/fonts/9x18.bdf")) {
        std::cerr << "Error: couldn't load font\n";
        exit(-1);
      }
      Color Blue(0, 0, 255);
      Color White(200, 200, 200);
      Color Yellow(255, 255, 0);
      Color Black(0, 0, 0);
      int v = 0;
      FrameCanvas *Frame = M->CreateFrameCanvas();
      while (running()) {
        //DrawText(canvas(), F, -2, F.baseline()-2, Yellow, "123456");
        v++;
        BigNumber::Render(Frame, 1, (v / 10) % 100, White, &Black);
        TD.Render(Frame);
        Frame = M->SwapOnVSync(Frame);
      }
    }
private:
    RGBMatrix *M;
    TimeDisplay TD;
};

int main(int argc, char *argv[]) {
    GPIO IO;

    if (!IO.Init()) {
        std::cerr << "Error: Could not init GPIO. Try again with 'sudo'?\n";
        exit(-1);
    }

    auto Matrix = std::unique_ptr<RGBMatrix>(new RGBMatrix(&IO, 32, 2, 1));
    Matrix->SetPWMBits(4);

    GameModel Model;

    auto Display = std::unique_ptr<GameDisplay>(new GameDisplay(&*Matrix, &Model));
    Display->Start();

    sleep(INT_MAX);
}