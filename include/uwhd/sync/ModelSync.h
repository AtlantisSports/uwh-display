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
#include <vector>

struct ModelSync {
  virtual ~ModelSync() {};
  virtual void Init() = 0;
  virtual void setMgr(GameModelManager *M) = 0;
  virtual GameModelManager &getMgr() = 0;
};

//struct ModelSyncPeer {
//  virtual std::string name() const = 0;
//  virtual std::string addr() const = 0;
//};

struct ModelSyncServer : public ModelSync {
  virtual ~ModelSyncServer() {};
  //virtual std::vector<ModelSyncPeer*> peers() = 0;
  //virtual void claimPeer(const ModelSyncPeer *P) = 0;
  //virtual void getPeerWallClock(const ModelSyncPeer *P) = 0;
  //virtual void setPeerWallClock(const ModelSyncPeer *P) = 0;
  //virtual void adjustPeerWallClock(const ModelSyncPeer *P, int Delta) = 0;
};

ModelSync *CreateSocketServer(const std::string &Port);
ModelSync *CreateSocketClient(const std::string &Host, const std::string &Port);
ModelSyncServer *CreateXBeeSyncServer();
ModelSync *CreateXBeeSyncClient();

#endif
