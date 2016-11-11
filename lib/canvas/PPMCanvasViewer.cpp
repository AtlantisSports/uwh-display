
#include "uwhd/canvas/PPMCanvasViewer.h"

#include "uwhd/canvas/Canvas.h"
#include "uwhd/canvas/CanvasViewer.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <ostream>
#include <sstream>

namespace {

char maxValue(UWHDCanvas *C) {
  char Max = 0;
  C->forEach([&](unsigned X, unsigned Y) {
    UWHDPixel &V = C->at(X, Y);
    Max = std::max(Max, V.r);
    Max = std::max(Max, V.g);
    Max = std::max(Max, V.b);
  });
  return Max;
}

void printPPM(std::ostream &OS, UWHDCanvas *C) {
  // See: http://netpbm.sourceforge.net/doc/pgm.html

  // 1: Magic number
  // 2: Whitespace
  // 3: Width
  // 4: Whitespace
  // 5: Height
  // 6: Whitespace
  // 7: Max Value
  // 8: Whitespace
  OS << "P6\n"
     << C->w << " " << C->h << "\n"
     << (int)maxValue(C) << "\n";

  // 9: Image Data
  for (unsigned Y = 0, YE = C->h; Y != YE; ++Y) {
    for (unsigned X = 0, XE = C->w; X != XE; ++X) {
      UWHDPixel &V = C->at(X, Y);
      OS << (int)V.r << " " << (int)V.g << " " << (int)V.b;
      if (X + 1 != XE)
        OS << "  ";
    }
    OS << "\n";
  }
}

class PPMCanvasViewer : public UWHDCanvasViewer {
public:
  PPMCanvasViewer(std::fstream &&OS) : OS(std::move(OS)) {}

  ~PPMCanvasViewer() {
    OS.flush();
    OS.close();
  }

  void show(UWHDCanvas *C) override {
    printPPM(OS, C);
    OS.flush();
  }

private:
  std::fstream OS;
};

} // namespace

std::string asPPMString(UWHDCanvas *C) {
  std::stringstream SS;
  printPPM(SS, C);
  return SS.str();
}

UWHDCanvasViewer *createPPMCanvasViewer(const char *FileName) {
  return new PPMCanvasViewer(std::fstream(FileName, std::fstream::out));
}
