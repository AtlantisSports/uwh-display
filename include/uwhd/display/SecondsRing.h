//===-- SecondsRing.h - Renders a Snake-like Seconds Hand ---------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//


#ifndef SECONDSRING_H
#define SECONDSRING_H

namespace rgb_matrix {
    class Canvas;
    class Color;
}

class SecondsRing {
public:
  static void Render(rgb_matrix::Canvas *Canvas,
                     unsigned Display,
                     unsigned Seconds,
                     const rgb_matrix::Color &FG,
                     const rgb_matrix::Color *BG = nullptr);

private:
  static void WriteSegment(rgb_matrix::Canvas *Canvas,
                           unsigned x,
                           unsigned y,
                           const rgb_matrix::Color &FG,
                           const rgb_matrix::Color *BG,
                           bool Growing,
                           bool IsTail);

};

#endif
