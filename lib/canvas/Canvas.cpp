#include "uwhd/canvas/Canvas.h"

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <ostream>

namespace {

unsigned clamp(unsigned Min, unsigned V, unsigned Max) {
  return std::max(Min, std::min(V, Max));
}

} // namespace

::std::ostream& operator<<(::std::ostream &OS, const UWHDPixel &V) {
  return OS << "(" << clamp(0, V.r, 255) << ","
                   << clamp(0, V.g, 255) << ","
                   << clamp(0, V.b, 255) << ")";
}

UWHDCanvas *UWHDCanvas::create(unsigned W, unsigned H) {
  unsigned Pixels = W * H;
  size_t TrailPadding = Pixels * sizeof(UWHDPixel);
  UWHDCanvas *C = static_cast<UWHDCanvas*>(malloc(sizeof(UWHDCanvas) +
                                                  TrailPadding));
  C->w = W;
  C->h = H;
  return C;
}

void UWHDCanvas::fill(UWHDPixel V) {
  forEach([=](unsigned X, unsigned Y) {
    at(X, Y) = V;
  });
}
