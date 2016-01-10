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

#include <memory>
#include <string>

namespace uwhtimer {

class ModelSync {
public:
  virtual void Init() = 0;

  virtual void PushModel(GameModel M) = 0;

  virtual GameModel PullModel() = 0;

  virtual void setMgr(GameModelManager *M) = 0;
  virtual GameModelManager &getMgr() = 0;

};

std::unique_ptr<ModelSync> CreateSocketServer(const std::string &Port);
std::unique_ptr<ModelSync> CreateSocketClient(const std::string &Host,
                                              const std::string &Port);

}

#endif
