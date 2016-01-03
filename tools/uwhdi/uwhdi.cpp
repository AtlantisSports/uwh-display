//===-- uwhdi.cpp - Entry-Point for the interactive uwh-display ---- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/display/GameDisplay.h"

#include "uwhd/sync/ModelSync.h"

#include <led-matrix.h>
#include <gpio.h>

#include <climits>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <unistd.h>

using namespace rgb_matrix;
using namespace uwhtimer;

int main(int argc, char *argv[]) {
  GPIO IO;

  if (!IO.Init()) {
      std::cerr << "Error: Could not init GPIO. Try again with 'sudo'?\n";
      exit(-1);
  }

  auto Matrix = std::unique_ptr<RGBMatrix>(new RGBMatrix(&IO, 32, 3, 1));
  Matrix->SetPWMBits(11);

  auto Display = std::unique_ptr<GameDisplay>(new GameDisplay(&*Matrix));
  Display->Start();

  auto BTSync = ModelSync::CreateSocketServer("5555");
  BTSync->Init();

  while (true) {
    sleep(1);
  }

  return 0;
}
