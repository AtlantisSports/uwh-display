//===-- Console.cpp - Console utils for the interactive uwhd ------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/console/Console.h"

#include "uwhd/model/GameModel.h"

#include <iostream>
#include <sstream>

using namespace uwhtimer;

#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"
#define ANSI_RESET   "\x1b[0m"

bool Console::ParseLine(std::string I) {
  if (I.size() == 0)
    return false;

  switch (I[0]) {
  case 'Q':
  case 'q':
    return true;

  case 'B':
  case 'b': {
    std::stringstream SS;
    SS << I.substr(1);
    GameModel Cur = *M.getModel();
    int Score;
    SS >> Score;
    Cur.BlackScore = Score;
    M.setModel(Cur);
    return false;
  }

  case 'W':
  case 'w': {
    std::stringstream SS;
    SS << I.substr(1);
    GameModel Cur = *M.getModel();
    int Score;
    SS >> Score;
    Cur.WhiteScore = Score;
    M.setModel(Cur);
    return false;
  }

  case 'T':
  case 't': {
    std::stringstream SS;
    SS << I.substr(1);
    GameModel Cur = *M.getModel();
    int Time;
    SS >> Time;
    Cur.GameClockSecs = Time;
    M.setModel(Cur);
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
    if (!GameModel::deSerialize(I, New)) {
      M.setModel(New);
      return false;
    }
  }
  default: { /* Fall Through */
    std::cerr << "Parse error on: '" << I << "'\n";
    return false;
  }
  }

  return true;
}

void Console::Loop() {
  while (true) {
    std::cout << ANSI_CYAN << "[uwhdi] " << ANSI_RESET;

    std::string I;
    std::getline(std::cin, I);

    if (ParseLine(I)) {
      std::cout << "Quitting...\n";
      break;
    }
  }
}

