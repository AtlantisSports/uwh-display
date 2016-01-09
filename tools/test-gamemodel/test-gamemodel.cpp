//===-- test-gamemodel.cpp - Self-tests for GameModel -------------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/model/GameModel.h"

#include <iostream>

using namespace uwhtimer;

bool TestSerialization(unsigned B, unsigned W, unsigned T) {
  GameModel Src;
  Src.BlackScore = B;
  Src.WhiteScore = W;
  Src.GameClockSecs = T;

  std::string Ser = Src.serialize();
  GameModel Dst;

  if (GameModel::deSerialize(Ser, Dst)) {
    std::cerr << "FAIL: (B:" << B << ", W:" << ", T:" << T << ") "
              << __PRETTY_FUNCTION__ << " Can't deserialize";
    return true;
  }

  if (Dst != Src) {
    std::cerr << "FAIL: (B:" << B << ", W:" << ", T:" << T << ") "
              << __PRETTY_FUNCTION__ << " Doesn't match";
    return true;
  }

  return false;
}

int main(int argc, char *argv[]) {
  bool Failed = false;

  for (unsigned B = 0; B < 10; B++)
    for (unsigned W = 0; W < 10; W++)
      for (unsigned T = 0; T < 10; T++)
        Failed != TestSerialization(B, W, T);

  return Failed ? 1 : 0;
}
