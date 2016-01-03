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

namespace uwhtimer {

class ModelSync {
public:
  virtual void Init() = 0;

  virtual void PushModel(GameModel M) = 0;

  virtual GameModel PullModel() = 0;

  static std::unique_ptr<ModelSync> CreateSocket(bool IsMaster, uint16_t Port);
};

}

#endif
