#ifndef UWHD_CANVAS_LEDCANVASVIEWER_H
#define UWHD_CANVAS_LEDCANVASVIEWER_H

#include <string>

struct UWHDCanvasViewer;
struct UWHDCanvas;

std::string asPPMString(UWHDCanvas *C);

UWHDCanvasViewer *createPPMCanvasViewer(const char *FileName);

#endif

