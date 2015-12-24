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

namespace uwhtimer {

class BigNumber {
public:
  static void RenderQuarter(rgb_matrix::Canvas *Canvas,
                         unsigned Display,
                         unsigned Value,
                         unsigned X, unsigned Y,
                         const rgb_matrix::Color &FG,
                         const rgb_matrix::Color *BG = nullptr);

  static void RenderQuarterSingle(rgb_matrix::Canvas *Canvas,
                               unsigned Display,
                               unsigned Value,
                               unsigned X, unsigned Y,
                               const rgb_matrix::Color &FG,
                               const rgb_matrix::Color *BG = nullptr);

  static void RenderQuarterDouble(rgb_matrix::Canvas *Canvas,
                               unsigned Display,
                               unsigned Value,
                               unsigned X, unsigned Y,
                               const rgb_matrix::Color &FG,
                               const rgb_matrix::Color *BG = nullptr);

  static void RenderHalf(rgb_matrix::Canvas *Canvas,
                         unsigned Display,
                         unsigned Value,
                         unsigned X, unsigned Y,
                         const rgb_matrix::Color &FG,
                         const rgb_matrix::Color *BG = nullptr);

  static void RenderHalfSingle(rgb_matrix::Canvas *Canvas,
                               unsigned Display,
                               unsigned Value,
                               unsigned X, unsigned Y,
                               const rgb_matrix::Color &FG,
                               const rgb_matrix::Color *BG = nullptr);

  static void RenderHalfDouble(rgb_matrix::Canvas *Canvas,
                               unsigned Display,
                               unsigned Value,
                               unsigned X, unsigned Y,
                               const rgb_matrix::Color &FG,
                               const rgb_matrix::Color *BG = nullptr);

  static void Render(rgb_matrix::Canvas *Canvas,
                     unsigned Display,
                     unsigned Value,
                     const rgb_matrix::Color &FG,
                     const rgb_matrix::Color *BG = nullptr);

  static void RenderSingle(rgb_matrix::Canvas *Canvas,
                     unsigned Display,
                     unsigned Value,
                     const rgb_matrix::Color &FG,
                     const rgb_matrix::Color *BG = nullptr);

  static void RenderDouble(rgb_matrix::Canvas *Canvas,
                     unsigned Display,
                     unsigned Value,
                     const rgb_matrix::Color &FG,
                     const rgb_matrix::Color *BG = nullptr);
};

}

#endif
