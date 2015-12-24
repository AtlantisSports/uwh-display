#ifndef RENDERELEMENT_H
#define RENDERELEMENT_H

namespace rgb_matrix {
  class Canvas;
}

namespace uwhtimer {
struct RenderElement {
  virtual void Render(rgb_matrix::Canvas *C) = 0;
};
}

#endif
