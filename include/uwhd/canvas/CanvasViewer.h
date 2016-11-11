#ifndef UWHD_CANVAS_CANVASVIEWER_H
#define UWHD_CANVAS_CANVASVIEWER_H

struct UWHDCanvas;

struct UWHDCanvasViewer {
  virtual ~UWHDCanvasViewer() {}
  virtual void show(UWHDCanvas *C) = 0;
};

#endif
