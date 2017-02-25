//===-- TimeDisplay.cpp - Renders a 32x32 Time Panel --------------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/display/Status.h"

#include "uwhd/canvas/Canvas.h"
#include "uwhd/display/BigNumber.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>

const UWHDPixel HostnameColor = UWHDPixel(200, 0, 255);
const UWHDPixel BGColor       = UWHDPixel(  0, 0, 0);

void renderStatus(UWHDCanvas *C) {
  if (FILE *FP = popen("/bin/hostname", "r")) {
    char Hostname[17];
    fgets(Hostname, sizeof(Hostname)-1, FP);
    std::transform(std::begin(Hostname), std::end(Hostname),
                   std::begin(Hostname), ::toupper);
    BigNumber::printf(C, 0, 0, HostnameColor, &BGColor, "%s", Hostname);
    pclose(FP);
  } else
    BigNumber::printf(C, 0, 0, HostnameColor, &BGColor, "*ERR*");
}
