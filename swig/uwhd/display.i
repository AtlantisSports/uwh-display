%module display
%{
#include <gpio.h>
#include <canvas.h>
#include <thread.h>
#include <transformer.h>
#include <threaded-canvas-manipulator.h>
#include <led-matrix.h>
#include "uwhd/display/BigNumber.h"
#include "uwhd/display/GameDisplay.h"
#include "uwhd/display/SecondsRing.h"
#include "uwhd/display/TimeDisplay.h"
%}

%include "stdint.i"
%include <gpio.h>
%include <canvas.h>
%include <thread.h>
%include <transformer.h>
%include <threaded-canvas-manipulator.h>
%include <led-matrix.h>
%include "uwhd/display/BigNumber.h"
%include "uwhd/display/GameDisplay.h"
%include "uwhd/display/SecondsRing.h"
%include "uwhd/display/TimeDisplay.h"

