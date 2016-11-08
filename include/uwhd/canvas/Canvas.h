#ifndef UWHD_DISPLAY_CANVAS_H
#define UWHD_DISPLAY_CANVAS_H

struct UWHDPixel {
  union {
    struct {
      char r;
      char g;
      char b;
    };
    char data[3];
  };
};

struct UWHDCanvas {
  unsigned w;
  unsigned h;
  UWHDPixel data[];

  static UWHDCanvas *create(unsigned W, unsigned H);

  inline void set(unsigned X, unsigned Y, UWHDPixel V) {
    get(X, Y) = V;
  }

  inline UWHDPixel &get(unsigned X, unsigned Y) {
    return data[X + Y * h];
  }
};

#endif
