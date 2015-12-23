#ifndef SECONDSRING_H
#define SECONDSRING_H

namespace rgb_matrix {
    class Canvas;
    class Color;
}

namespace uwhtimer {

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

}

#endif
