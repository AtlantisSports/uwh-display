#include "BigNumber.h"
#include "SecondsRing.h"

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
    GameDisplay(Canvas *Canvas, GameModel *Model)
        : ThreadedCanvasManipulator(Canvas) {}

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
      while (running()) {
        //DrawText(canvas(), F, -2, F.baseline()-2, Yellow, "123456");
        v++;
        if (v>=999) v = 0;
        BigNumber::Render(canvas(), 0, v / 100, Blue, &Black);
        BigNumber::Render(canvas(), 1, v % 100, White, &Black);
        SecondsRing::Render(canvas(), 0, v, Yellow, &Black);
        usleep(1000000);
      }
    }
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
