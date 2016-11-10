
#include "uwhd/canvas/PPMCanvasViewer.h"

#include "uwhd/canvas/Canvas.h"
#include "uwhd/canvas/CanvasViewer.h"

#include <algorithm>
#include <cassert>
#include <cstdio>

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

void printPPM(FILE *F, UWHDCanvas *C) {
  // See: http://netpbm.sourceforge.net/doc/pgm.html

  // 1: Magic number
  // 2: Whitespace
  // 3: Width
  // 4: Whitespace
  // 5: Height
  // 6: Whitespace
  // 7: Max Value
  // 8: Whitespace
  fprintf(F, "%s\n%d %d\n%d\n", "P6", C->w, C->h, maxValue(C));

  // 9: Image Data
  for (unsigned Y = 0, YE = C->h; Y != YE; ++Y) {
    for (unsigned X = 0, XE = C->w; X != XE; ++X) {
      UWHDPixel &V = C->at(X, Y);
      fprintf(F, "%c %c %c", V.r, V.g, V.b);
      if (X + 1 != XE)
        fprintf(F, "%s", " ");
    }
    fprintf(F, "%s", "\n");
  }
}

class PPMCanvasViewer : public UWHDCanvasViewer {
public:
  PPMCanvasViewer(FILE *F, bool Close) : F(F), Close(Close) {}

  ~PPMCanvasViewer() {
    assert(F);
    if (Close)
      fclose(F);
  }

  void show(UWHDCanvas *C) override {
    assert(F);
    printPPM(F, C);
  }

private:
  FILE *F;
  bool Close;
};

} // namespace

UWHDCanvasViewer *createPPMCanvasViewer(const char *FileName) {
  FILE *F = fopen(FileName, "w");

  if (!F)
    return nullptr;

  return new PPMCanvasViewer(F, /*Close=*/true);
}

UWHDCanvasViewer *createPPMCanvasViewer(FILE *F) {
  return new PPMCanvasViewer(F, /*Close=*/false);
}
