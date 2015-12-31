//===-- RenderElement.h - Interface for Displayable Elements ------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

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
