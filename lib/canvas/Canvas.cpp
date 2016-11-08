#include "uwhd/canvas/Canvas.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>

UWHDCanvas *UWHDCanvas::create(unsigned W, unsigned H) {
  unsigned Pixels = W * H;
  size_t TrailPadding = Pixels * sizeof(UWHDPixel);
  UWHDCanvas *C = static_cast<UWHDCanvas*>(malloc(sizeof(UWHDCanvas) +
                                                  TrailPadding));
  C->w = W;
  C->h = H;
  return C;
}

