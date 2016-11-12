//===-- BigNumber.h - Large Format Number Rendering  --------------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//


#ifndef BIGNUMBER_H
#define BIGNUMBER_H

class UWHDCanvas;
class UWHDPixel;

class BigNumber {
public:
  enum Font {
  Ascii5x7,
  HexUpper5x7,
  Digit5x7,
  Digit11x20,
  Digit15x29
  };

  static void Render(UWHDCanvas *C,
                     unsigned Display,
                     unsigned char Digit,
                     unsigned XOffs, unsigned YOffs,
                     enum BigNumber::Font F,
                     const UWHDPixel &FG,
                     const UWHDPixel *BG = nullptr);

  static void printf(UWHDCanvas *C,
                     unsigned X, unsigned Y,
                     const UWHDPixel &FG,
                     const UWHDPixel *BG,
                     const char *Fmt, ...);
};

#endif
