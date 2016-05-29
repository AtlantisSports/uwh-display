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

bool CheckRoundTripSerialize(unsigned B, unsigned W, unsigned T,
                             bool ClockRunning, GameModel::GameState S) {
  GameModel Src;
  Src.BlackScore = B;
  Src.WhiteScore = W;
  Src.GameClockSecs = T;
  Src.ClockRunning = ClockRunning;
  Src.State = S;

  std::string Ser = Src.serialize();
  GameModel Dst;

  if (GameModel::deSerialize(Ser, Dst)) {
    std::cerr << "FAIL: (B:" << B << ", W:" << ", T:" << T
                             << ", R:" << (ClockRunning?"Y":"N") << ")\n"
              << __PRETTY_FUNCTION__ << " Can't deserialize\n";
    return true;
  }

  if (Dst != Src) {
    std::cerr << "FAIL: (B:" << B << ", W:" << W << ", T:" << T << ")\n"
              << Dst.dump() << "\n"
              << Src.dump() << "\n"
              << "      " << __PRETTY_FUNCTION__ << " Doesn't match\n";
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
      M1.GameClockSecs == M2.GameClockSecs &&
      M1.ClockRunning == M2.ClockRunning &&
      M1.State == M2.State) {
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
      M1.GameClockSecs != M2.GameClockSecs ||
      M1.ClockRunning != M2.ClockRunning ||
      M1.State != M2.State) {
    std::cerr << "FAIL: " << M1.serialize() << " != " << M2.serialize() << " "
              << __PRETTY_FUNCTION__ << " inequality\n";
    return true;
  }

  return false;
}


int main(int argc, char *argv[]) {
  bool Failed = false;

  Failed |= CheckRoundTripSerialize(42, 37, 104, false, GameModel::WallClock);
  Failed |= CheckRoundTripSerialize(42, 37, 104, false, GameModel::NormalPlay);
  Failed |= CheckRoundTripSerialize(42, 37, 104, false, GameModel::HalfTime);
  Failed |= CheckRoundTripSerialize(42, 37, 104, false, GameModel::RefTimeOut);
  Failed |= CheckRoundTripSerialize(42, 37, 104, false, GameModel::TeamTimeOut);
  Failed |= CheckRoundTripSerialize(42, 37, 104, false, GameModel::GameOver);

  for (unsigned B = 0; B < 10; B++)
    for (unsigned W = 0; W < 10; W++)
      for (unsigned T = 0; T < 10; T++) {
        Failed |= CheckRoundTripSerialize(B, W, T, false, GameModel::WallClock);
        Failed |= CheckRoundTripSerialize(B, W, T, true,  GameModel::WallClock);
      }

  Failed |= CheckRoundTripSerialize(99, 99, 9999, true, GameModel::WallClock);
  Failed |= CheckRoundTripSerialize(99, 99, 999999, false, GameModel::WallClock);
  Failed |= CheckRoundTripSerialize(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, true, GameModel::WallClock);

  GameModel M1;
  M1.BlackScore = 1;
  M1.WhiteScore = 2;
  M1.GameClockSecs = 15;
  M1.ClockRunning = true;
  M1.State = GameModel::WallClock;

  GameModel M2 = M1;
  Failed |= CheckEquals(M1, M2);
  Failed |= CheckEquals(M2, M1);

  M2 = M1;
  M2.WhiteScore = 4;
  Failed |= CheckNotEquals(M1, M2);
  Failed |= CheckNotEquals(M2, M1);

  M2 = M1;
  M2.BlackScore = 4;
  Failed |= CheckNotEquals(M1, M2);
  Failed |= CheckNotEquals(M2, M1);

  M2 = M1;
  M2.ClockRunning = false;
  Failed |= CheckNotEquals(M1, M2);
  Failed |= CheckNotEquals(M2, M1);

  M2 = M1;
  M2.ClockRunning = false;
  M2.WhiteScore = 4;
  Failed |= CheckNotEquals(M1, M2);
  Failed |= CheckNotEquals(M2, M1);

  M2 = M1;
  M2.ClockRunning = false;
  M2.BlackScore = 4;
  Failed |= CheckNotEquals(M1, M2);
  Failed |= CheckNotEquals(M2, M1);

  M2 = M1;
  M2.WhiteScore = 5;
  M2.BlackScore = 4;
  Failed |= CheckNotEquals(M1, M2);
  Failed |= CheckNotEquals(M2, M1);

  M2 = M1;
  M2.ClockRunning = false;
  M2.WhiteScore = 5;
  M2.BlackScore = 4;
  Failed |= CheckNotEquals(M1, M2);
  Failed |= CheckNotEquals(M2, M1);

  M2 = M1;
  M2.State = GameModel::NormalPlay;
  Failed |= CheckNotEquals(M1, M2);
  Failed |= CheckNotEquals(M2, M1);

  return Failed ? 1 : 0;
}
