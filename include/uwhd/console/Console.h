//===-- Console.h -------------------------------------------------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>

namespace uwhtimer {
class GameDisplay;

class Console {
public:
  Console(GameDisplay &D) : D(D) {}

  bool ParseLine(std::string I);
  void Loop();

private:
  GameDisplay &D;
};

} // namespace uwtimer
#endif
