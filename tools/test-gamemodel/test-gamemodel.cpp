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

bool CheckRoundTripSerialize(unsigned B, unsigned W, unsigned T) {
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

bool CheckNotEquals(GameModel M1, GameModel M2) {
  if (M1 == M2) {
    std::cerr << "FAIL: " << M1.serialize() << " == " << M2.serialize() << " "
              << __PRETTY_FUNCTION__ << " operator==\n";
    return true;
  }

  if (!(M1 != M2)) {
    std::cerr << "FAIL: " << M1.serialize() << " == " << M2.serialize() << " "
              << __PRETTY_FUNCTION__ << " operator!=\n";
    return true;
  }

  if (M1.BlackScore == M2.BlackScore &&
      M1.WhiteScore == M2.WhiteScore &&
      M1.GameClockSecs == M2.GameClockSecs) {
    std::cerr << "FAIL: " << M1.serialize() << " == " << M2.serialize() << " "
              << __PRETTY_FUNCTION__ << " equality\n";
    return true;
  }

  return false;
}

bool CheckEquals(GameModel M1, GameModel M2) {
  if (M1 != M2) {
    std::cerr << "FAIL: " << M1.serialize() << " != " << M2.serialize() << " "
              << __PRETTY_FUNCTION__ << " operator!=\n";
    return true;
  }

  if (!(M1 == M2)) {
    std::cerr << "FAIL: " << M1.serialize() << " != " << M2.serialize() << " "
              << __PRETTY_FUNCTION__ << " operator==\n";
    return true;
  }

  if (M1.BlackScore != M2.BlackScore ||
      M1.WhiteScore != M2.WhiteScore ||
      M1.GameClockSecs != M2.GameClockSecs) {
    std::cerr << "FAIL: " << M1.serialize() << " != " << M2.serialize() << " "
              << __PRETTY_FUNCTION__ << " inequality\n";
    return true;
  }

  return false;
}


int main(int argc, char *argv[]) {
  bool Failed = false;

  for (unsigned B = 0; B < 10; B++)
    for (unsigned W = 0; W < 10; W++)
      for (unsigned T = 0; T < 10; T++)
        Failed != CheckRoundTripSerialize(B, W, T);

  Failed != CheckRoundTripSerialize(99, 99, 9999);
  Failed != CheckRoundTripSerialize(99, 99, 999999);
  Failed != CheckRoundTripSerialize(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);

  GameModel M1;
  M1.BlackScore = 1;
  M1.WhiteScore = 2;
  M1.GameClockSecs = 15;

  GameModel M2 = M1;
  Failed != CheckEquals(M1, M2);

  M1.BlackScore = 2;
  Failed != CheckNotEquals(M1, M2);

  return Failed ? 1 : 0;
}
