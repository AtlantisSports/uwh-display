//===-- ModelSync.h - Wireless Syncing of GameModel ---------------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//


#ifndef MODELSYNC_H
#define MODELSYNC_H

#include "uwhd/model/GameModel.h"

#include <string>

struct ModelSync {
  virtual void Init() = 0;
  virtual void setMgr(GameModelManager *M) = 0;
  virtual GameModelManager &getMgr() = 0;
};

ModelSync *CreateSocketServer(const std::string &Port);
ModelSync *CreateSocketClient(const std::string &Host, const std::string &Port);
ModelSync *CreateXBeeSync(bool ThisIsCoordinator);

#endif
