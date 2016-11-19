%module display
%{
#include "uwhd/canvas/Canvas.h"
#include "uwhd/canvas/CanvasViewer.h"
#include "uwhd/canvas/PPMCanvasViewer.h"
#include "uwhd/canvas/LEDCanvasViewer.h"
%}

%nodefaultctor UWHDPixel;
%nodefaultctor UWHDCanvas;
%include "uwhd/canvas/Canvas.h"
%include "uwhd/canvas/CanvasViewer.h"
%include "uwhd/canvas/PPMCanvasViewer.h"
%include "uwhd/canvas/LEDCanvasViewer.h"

