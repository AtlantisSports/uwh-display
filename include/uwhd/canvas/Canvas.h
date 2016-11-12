#ifndef UWHD_DISPLAY_CANVAS_H
#define UWHD_DISPLAY_CANVAS_H

#include <cassert>
#include <iosfwd>

struct UWHDPixel {
  union {
    struct {
      char r;
      char g;
      char b;
    };
    char data[3];
  };

  UWHDPixel(char r, char g, char b) : r(r), g(g), b(b) {}
};

inline bool operator==(UWHDPixel LHS, UWHDPixel RHS) {
  return LHS.r == RHS.r &&
         LHS.g == RHS.g &&
         LHS.b == RHS.b;
}

inline bool operator!=(UWHDPixel LHS, UWHDPixel RHS) {
  return !(LHS == RHS);
}

::std::ostream& operator<<(::std::ostream& os, const UWHDPixel &V);

struct UWHDCanvas {
  unsigned w;
  unsigned h;
  UWHDPixel data[];

  static UWHDCanvas *create(unsigned W, unsigned H);

  inline UWHDPixel &at(unsigned X, unsigned Y) {
    assert(X < w && Y < h && "coords not in bounds?");
    return data[X + Y * w];
  }

  template<typename F>
  void forEach(F Func) {
    for (unsigned Y = 0, YE = h; Y != YE; ++Y)
      for (unsigned X = 0, XE = w; X != XE; ++X)
        Func(X, Y);
  }

  void fill(UWHDPixel V);
};

#endif
