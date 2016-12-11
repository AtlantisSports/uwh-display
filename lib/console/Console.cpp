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
#include <cctype>

#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"
#define ANSI_RESET   "\x1b[0m"

bool Console::ParseLine(std::string I) {
  if (I.size() == 0)
    goto Success;

  switch (I[0]) {
  case 'Q':
  case 'q':
    goto ExitLoop;

  case 'B':
  case 'b': {
    std::stringstream SS;
    SS << I.substr(1);
    int Score;
    SS >> Score;
    M.setBlackScore(Score);
    goto Success;
  }

  case 'W':
  case 'w': {
    std::stringstream SS;
    SS << I.substr(1);
    int Score;
    SS >> Score;
    M.setWhiteScore(Score);
    goto Success;
  }

  case 'T':
  case 't': {
    std::stringstream SS;
    SS << I.substr(1);
    int Time;
    SS >> Time;
    M.setGameClock(Time);
    goto Success;
  }

  case 'C':
  case 'c': {
    bool Running = M.gameClockRunning();
    M.setGameClockRunning(!Running);
    goto Success;
  }

  case 'H':
  case 'h': {
    std::cout << "Usage:\n"
              << "  B[0-9]+     - Set the Black Score\n"
              << "  W[0-9]+     - Set the White Score\n"
              << "  T[0-9]+     - Set the Game Clock\n"
              << "  C           - Start/Stop the Game Clock\n"
              << "  G[CFSHRWBO] - Set the Game State\n"
              << "    GC          - Wall Clock\n"
              << "    GF          - First Half\n"
              << "    GS          - Second Half\n"
              << "    GH          - Half Time\n"
              << "    GR          - Ref Time Out\n"
              << "    GW          - White Time Out\n"
              << "    GB          - Black Time Out\n"
              << "    GO          - Game Over\n"
              << "  P[NRWB]     - Set the Timeout State\n"
              << "    PN          - No Timeout\n"
              << "    PR          - Ref Timeout\n"
              << "    PW          - White Timeout\n"
              << "    PB          - Black Timeout\n"
              << "  H           - This menu\n"
              << "  Q           - Quit\n";
    goto Success;
  }

  case 'G':
  case 'g': {
    if (I.size() != 2)
      goto ParseError;

    switch (toupper(I[1])) {
    case 'C': M.setGameState(GameModel::GS_WallClock); goto Success;
    case 'F': M.setGameState(GameModel::GS_FirstHalf); goto Success;
    case 'S': M.setGameState(GameModel::GS_SecondHalf); goto Success;
    case 'H': M.setGameState(GameModel::GS_HalfTime); goto Success;
    case 'O': M.setGameState(GameModel::GS_GameOver); goto Success;
    default:
      goto ParseError;
    }
  }

  case 'P':
  case 'p': {
    if (I.size() != 2)
      goto ParseError;

    switch (toupper(I[1])) {
    case 'N': M.setTimeoutState(GameModel::TS_None); goto Success;
    case 'R': M.setTimeoutState(GameModel::TS_RefTimeout); goto Success;
    case 'W': M.setTimeoutState(GameModel::TS_WhiteTimeout); goto Success;
    case 'B': M.setTimeoutState(GameModel::TS_BlackTimeout); goto Success;
    default:
      goto ParseError;
    }
  }

  case 'S': {
    GameModel New;
    if (!GameModel::deSerialize(I, New)) {
      M.setModel(New);
      goto Success;
    }
    goto ParseError;
  }

  default:
    goto ParseError;
  }

ParseError:
  std::cerr << "Parse error on: '" << I << "'\n";
  return false;

Success:
  return false;

ExitLoop:
  return true;
}

void Console::Loop() {
  while (true) {
    std::cout << M.getModel().dump();
    std::cout << ANSI_CYAN << "[uwhdi] " << ANSI_RESET;

    std::string I;
    std::getline(std::cin, I);

    if (ParseLine(I)) {
      std::cout << "Quitting...\n";
      break;
    }
  }
}

