#include "uwhd/config/Config.h"

#include "uwhd/canvas/Canvas.h"
#include "uwhd/canvas/CanvasViewer.h"
#include "uwhd/canvas/PPMCanvasViewer.h"

#ifdef UWHD_BUILD_DISPLAY

#include <led-matrix.h>
#include <graphics.h>
#include <canvas.h>
#include <memory>

using rgb_matrix::RGBMatrix;
using rgb_matrix::GPIO;
using rgb_matrix::FrameCanvas;

class LEDCanvasViewer : public UWHDCanvasViewer {
public:
  LEDCanvasViewer(RGBMatrix *Mtx)
    : Mtx(Mtx)
    , Frame(Mtx->CreateFrameCanvas())
  {}

  virtual void show(UWHDCanvas *C) override {
    C->forEach([&](unsigned X, unsigned Y) {
      UWHDPixel &V = C->at(X, Y);
      Frame->SetPixel(X, Y, V.r, V.g, V.b);
    });
    Frame = Mtx->SwapOnVSync(Frame);
  }

private:
  RGBMatrix *Mtx;
  FrameCanvas *Frame;
};

#endif // UWHD_BUILD_DISPLAY

UWHDCanvasViewer *createLEDCanvasViewer() {
#ifdef UWHD_BUILD_DISPLAY
  static GPIO IO;
  if (!IO.Init()) {
    printf("error: Could not init GPIO. Are you running with 'sudo'?\n");
    return nullptr;
  }

  auto Matrix = new RGBMatrix(&IO, 32, 3, 1);
  Matrix->SetPWMBits(11);

  return new LEDCanvasViewer(Matrix);
#else
  return nullptr;
#endif
}

