
#include "uwhd/canvas/PPMCanvasViewer.h"

#include "uwhd/canvas/Canvas.h"
#include "uwhd/canvas/CanvasViewer.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <ostream>
#include <sstream>
#include <cstdint>
#include <iomanip>

namespace {

unsigned clamp(unsigned Min, unsigned V, unsigned Max) {
  return std::max(Min, std::min(V, Max));
}

unsigned maxValue(UWHDCanvas *C) {
  unsigned Max = 0;
  C->forEach([&](unsigned X, unsigned Y) {
    UWHDPixel &V = C->at(X, Y);
    Max = std::max(Max, clamp(0, V.r, 255));
    Max = std::max(Max, clamp(0, V.g, 255));
    Max = std::max(Max, clamp(0, V.b, 255));
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
  OS << "P3\n"
     << C->w << " " << C->h << "\n"
     << maxValue(C) << "\n";

  // 9: Image Data
  for (unsigned Y = 0, YE = C->h; Y != YE; ++Y) {
    for (unsigned X = 0, XE = C->w; X != XE; ++X) {
      UWHDPixel &V = C->at(X, Y);
      OS << std::setfill(' ')
         << std::setw(3) << clamp(0, V.r, 255) << " "
         << std::setw(3) << clamp(0, V.g, 255) << " "
         << std::setw(3) << clamp(0, V.b, 255);
      if (X + 1 != XE)
        OS << "  ";
    }
    OS << "\n";
  }
}

} // namespace

std::string asPPMString(UWHDCanvas *C) {
  std::stringstream SS;
  printPPM(SS, C);
  return SS.str();
}

UWHDCanvas *createCanvasFromPPMString(const std::string &Str) {
  std::stringstream SS(Str);

  std::string Magic;
  SS >> Magic;
  if (Magic != "P3")
    return nullptr;

  unsigned W, H, Max;
  SS >> W >> H >> Max;

  UWHDCanvas *C = UWHDCanvas::create(W, H);

  if (!C)
    return nullptr;

  C->forEach([&](unsigned X, unsigned Y) {
    unsigned R, G, B;
    SS >> R >> G >> B;
    C->at(X, Y) = UWHDPixel(R, G, B);
  });

  return C;
}

namespace {

class PPMCanvasViewer : public UWHDCanvasViewer {
public:
  PPMCanvasViewer(const char *FileName) : OS(FileName, std::fstream::out) {}

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

UWHDCanvasViewer *createPPMCanvasViewer(const char *FileName) {
  return new PPMCanvasViewer(FileName);
}
