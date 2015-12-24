#include "BigNumber.h"
#include "RenderElement.h"

#include "graphics.h"
#include "gpio.h"
#include "led-matrix.h"
#include "threaded-canvas-manipulator.h"

#include <ctime>

using namespace rgb_matrix;
using namespace uwhtimer;

class TimeDisplay : public RenderElement {
public:
    TimeDisplay(unsigned DisplayNum);
    void Render(rgb_matrix::Canvas *C) override;
private:
    unsigned DisplayNum;
};
