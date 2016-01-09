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
#include <sstream>
#include <unistd.h>

using namespace rgb_matrix;
using namespace uwhtimer;

bool ParseInput(std::string Input, GameDisplay &Display) {
  if (Input.size() == 0)
    return false;

  switch (Input[0]) {
  case 'Q':
  case 'q':
    return true;

  case 'B':
  case 'b': {
    std::stringstream SS;
    SS << Input.substr(1);
    GameModel Cur = *Display.getMgr().getModel();
    SS >> Cur.BlackScore;
    Display.getMgr().setModel(Cur);
    return false;
  }

  case 'W':
  case 'w': {
    std::stringstream SS;
    SS << Input.substr(1);
    GameModel Cur = *Display.getMgr().getModel();
    SS >> Cur.WhiteScore;
    Display.getMgr().setModel(Cur);
    return false;
  }

  case 'T':
  case 't': {
    std::stringstream SS;
    SS << Input.substr(1);
    GameModel Cur = *Display.getMgr().getModel();
    SS >> Cur.GameClockSecs;
    Display.getMgr().setModel(Cur);
    return false;
  }

  case 'H':
  case 'h': {
    std::cout << "Usage:\n"
              << "  B[0-9]+ - Set the Black Score\n"
              << "  W[0-9]+ - Set the White Score\n"
              << "  T[0-9]+ - Set the Game Clock\n"
              << "  H       - This menu\n"
              << "  Q       - Quit\n";
    return false;
  }

  case 'S': {
    GameModel New;
    if (!GameModel::deSerialize(Input, New)) {
      std::cout << New.dump() << "\n";
      Display.getMgr().setModel(New);
      return false;
    }
  }
  default: { /* Fall Through */
    std::cerr << "Parse error on: '" << Input << "'\n";
    return false;
  }
  }

  return true;
}

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

  while (true) {
    std::cout << "[uwhdi] ";

    std::string Input;
    std::getline(std::cin, Input);

    if (ParseInput(Input, *Display)) {
      std::cout << "Quitting...\n";
      break;
    }
  }

  return 0;
}
