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
#include "uwhd/config/Status.h"
#include "uwhd/display/BigNumber.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>

const UWHDPixel StatusColor1 = UWHDPixel(200, 0, 255);
const UWHDPixel StatusColor2 = UWHDPixel(255, 0, 200);
const UWHDPixel BGColor      = UWHDPixel(  0, 0,   0);

static const char *UWHDGitSha = UWHD_GIT_SHA;
static const char *UWHDGitTag = UWHD_GIT_TAG;
static const char *UWHDBuildDate = UWHD_BUILD_DATE;

void renderStatus(UWHDCanvas *C) {
  // Hostname
  if (FILE *FP = popen("/bin/hostname", "r")) {
    char Hostname[17];
    fgets(Hostname, sizeof(Hostname)-1, FP);
    std::transform(std::begin(Hostname), std::end(Hostname),
                   std::begin(Hostname), ::toupper);
    BigNumber::printf(C, 0, 0, StatusColor1, &BGColor, "%s", Hostname);
    pclose(FP);
  } else
    BigNumber::printf(C, 0, 0, StatusColor1, &BGColor, "*ERR*");

  // Git Version
  char ShortSha[17];
  memset(ShortSha, 0, sizeof(ShortSha));
  strncpy(ShortSha, UWHDGitSha, sizeof(ShortSha)-1);
  std::transform(std::begin(ShortSha), std::end(ShortSha),
                 std::begin(ShortSha), ::toupper);
  BigNumber::printf(C, 0, 8, StatusColor2, &BGColor, "%s", ShortSha);

#if UWHD_GIT_DIRTY
  BigNumber::printf(C, 0, 16, StatusColor1, &BGColor, "DIRTY WORK DIR");
#else
  // Git Tag
  char GitTag[17];
  memset(GitTag, 0, sizeof(GitTag));
  strncpy(GitTag, UWHDGitTag, sizeof(GitTag)-1);
  std::transform(std::begin(GitTag), std::end(GitTag),
                 std::begin(GitTag), ::toupper);
  BigNumber::printf(C, 0, 16, StatusColor1, &BGColor, "%s", GitTag);
#endif

  // Build Date
  BigNumber::printf(C, 0, 24, StatusColor2, &BGColor, "%s", UWHDBuildDate);
}
