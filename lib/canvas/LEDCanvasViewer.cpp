#include "uwhd/config/Config.h"

#include "uwhd/canvas/CanvasViewer.h"

#ifdef UWHD_BUILD_DISPLAY

#include <graphics.h>
#include <canvas.h>
#include <led-matrix.h>
#include <memory>

using namespace rgb_matrix;

class LEDCanvasViewer {
public:
  LEDCanvasViewer(std::unique_ptr<RGBMatrix> Mtx)
    , Mtx(std::move(Mtx))
  {}

  virtual void show(UWHDCanvas *C) `{
    FrameCanvas *Frame = Mtx->CreateFrameCanvas();
    C->forEach([&](unsigned X, unsigned Y) {
      UWHDPixel &V = C->at(X, Y);
      Frame->SetPixel(X, Y, V.r, V.g, V.b);
    });
    Frame = Mtx->SwapOnVSync(Frame);
  }

private:
  std::unique_ptr<RGBMatrix> Mtx;
};

#endif // UWHD_BUILD_DISPLAY

UWHDCanvasViewer *createLEDCanvasViewer() {
#ifdef UWHD_BUILD_DISPLAY
  static GPIO IO;
  if (!IO.Init())
    return nullptr;

  auto Matrix = std::unique_ptr<RGBMatrix>(new RGBMatrix(&IO, 32, 3, 1));
  Matrix->SetPWMBits(11);

  return new LEDCanvasViewer(std::move(Matrix));
#else
  return nullptr;
#endif
}

