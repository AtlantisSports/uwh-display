#include "uwhd/canvas/CanvasViewer.h"

#include <graphics.h>
#include <canvas.h>
#include <led-matrix.h>
#include <threaded-canvas-manipulator.h>

namespace rgb_matrix {
  class RGBMatrix;
}

using namespace rgb_matrix;

class GameDisplay : public rgb_matrix::ThreadedCanvasManipulator {
public:
  GameDisplay(rgb_matrix::RGBMatrix *Mtx)
    : ThreadedCanvasManipulator(Mtx)
    , Mtx(Mtx)
    , Mgr()
    , TD(1, Mgr)
  {}

  void Run();

  /// For the SWIG bindings, which can't tell the overloads apart:
  virtual void Start0() { Start(); }
  virtual void Start2(int RealtimePriority, uint32_t CPUAffinityMask) {
    Start(RealtimePriority, CPUAffinityMask);
  }

  GameModelManager &getMgr() { return Mgr; }

  /// For SWIG, which doesn't do the right thing for references
  /// FIXME: replace uses of the other getter with this one.
  GameModelManager *getMgr2() { return &Mgr; }

private:
  rgb_matrix::RGBMatrix *Mtx;
  GameModelManager Mgr;
  TimeDisplay TD;

public:
  static const rgb_matrix::Color WhiteTeamFG;
  static const rgb_matrix::Color WhiteTeamBG;
  static const rgb_matrix::Color BlackTeamFG;
  static const rgb_matrix::Color BlackTeamBG;
  static const rgb_matrix::Color Background;
};

void GameDisplay::Run() {
  FrameCanvas *Frame = Mtx->CreateFrameCanvas();
  while (running()) {
    GameModel M = Mgr.getModel();

    renderGameDisplay(M, Canvas);

    Canvas->forEach([&](unsigned X, unsigned Y) {
      UWHDPixel &V = Canvas->at(X, Y);
      Frame->SetPixel(X, Y, V.r, V.g, V.b);
    });

    Frame = Mtx->SwapOnVSync(Frame);
  }
}

UWHDCanvasViewer *createLEDCanvasViewer() {
  return nullptr;
}

