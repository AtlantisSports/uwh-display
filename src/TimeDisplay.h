#include "BigNumber.h"

#include "graphics.h"
#include "gpio.h"
#include "led-matrix.h"
#include "threaded-canvas-manipulator.h"

#include <ctime>

using namespace rgb_matrix;
using namespace uwhtimer;

class TimeDisplay : public ThreadedCanvasManipulator {
public:
    TimeDisplay(RGBMatrix *M, unsigned DisplayNum);
    void Run();
private:
    RGBMatrix *M;
    unsigned DisplayNum;
};
