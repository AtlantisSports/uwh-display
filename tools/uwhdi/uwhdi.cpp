//===-- uwhdi.cpp - Entry-Point for the interactive uwh-display ---- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/console/Console.h"
#include "uwhd/display/GameDisplay.h"
#include "uwhd/canvas/Canvas.h"
#include "uwhd/canvas/CanvasViewer.h"
#include "uwhd/canvas/LEDCanvasViewer.h"

#include <climits>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <unistd.h>

int main(int argc, char *argv[]) {
  GameModelManager Mgr;

  struct ListenAdapter final : public GameModelListener {
    ListenAdapter(UWHDCanvasViewer *Viewer, UWHDCanvas *Canvas)
      : Viewer(Viewer), Canvas(Canvas) {}
    UWHDCanvasViewer *Viewer;
    UWHDCanvas *Canvas;

    virtual void modelChanged(GameModel M) override {
      renderGameDisplay(2, M, Canvas);
      Viewer->show(Canvas);
    }
  };

  UWHDCanvasViewer *LEDPanel = createLEDCanvasViewer();
  UWHDCanvas *Canvas = UWHDCanvas::create(32 * 3, 32);
  ListenAdapter LA = { LEDPanel, Canvas };
  Mgr.registerListener(&LA);

  Console C(Mgr);
  C.Loop();

  return 0;
}
