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

namespace rgb_matrix {
    class Canvas;
    class Color;
}

class BigNumber {
public:
  enum Font {
  Digit5x7,
  Digit11x20,
  Digit15x29
  };

  static void Render(rgb_matrix::Canvas *Canvas,
                     unsigned Display,
                     unsigned char Digit,
                     unsigned XOffs, unsigned YOffs,
                     enum BigNumber::Font F,
                     const rgb_matrix::Color &FG,
                     const rgb_matrix::Color *BG = nullptr);
};

#endif
