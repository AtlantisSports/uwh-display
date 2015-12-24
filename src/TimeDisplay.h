#include "RenderElement.h"

#include <graphics.h>

#include <ctime>

namespace rgb_matrix {
class Canvas;
}

namespace uwhtimer {
class GameModel;
class GameModelManager;

class TimeDisplay : public RenderElement {
public:
    TimeDisplay(unsigned DisplayNum, GameModelManager &Mgr)
      : DisplayNum(DisplayNum)
      , Mgr(Mgr) {}

    void Render(rgb_matrix::Canvas *C) override;
private:
    unsigned DisplayNum;
    GameModelManager &Mgr;
    static const rgb_matrix::Color SecondsColor;
    static const rgb_matrix::Color MinutesColor;
    static const rgb_matrix::Color ColonColor;
    static const rgb_matrix::Color RingColor;
};

}
