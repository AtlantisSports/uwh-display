#ifndef BIGNUMBER_H
#define BIGNUMBER_H

namespace rgb_matrix {
    class Canvas;
    class Color;
}

namespace uwhtimer {

class BigNumber {
public:
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
