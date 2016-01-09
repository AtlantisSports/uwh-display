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
class GameModelManager;

class Console {
public:
  Console(GameModelManager &M) : M(M) {}

  bool ParseLine(std::string I);
  void Loop();

private:
  GameModelManager &M;
};

} // namespace uwtimer
#endif
