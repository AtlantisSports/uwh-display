//===-- uwhdc.cpp -------------------------------------------------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/console/Console.h"
#include "uwhd/sync/ModelSync.h"

using namespace uwhtimer;

int main(int argc, char *argv[]) {
  GameModelManager M;
  Console C(M);

  auto SyncClient = ModelSync::CreateSocketClient("127.0.0.1", "5555");
  SyncClient->Init();

  C.Loop();

  return 0;
}
