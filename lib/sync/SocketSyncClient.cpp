//===-- SocketSyncClient.cpp - Socket Syncing for GameModel -------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/sync/ModelSync.h"

#include "uwhd/model/GameModel.h"

#include <string>

#include <cassert>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

static const int MAX_EVENTS = 16;

using namespace uwhtimer;

class SocketSyncClient : public ModelSync {
public:
  SocketSyncClient(const std::string &Host, const std::string &Port);

  virtual void Init() override;

  virtual void PushModel(GameModel M) override;

  virtual GameModel PullModel() override;

  virtual void setMgr(GameModelManager *M) override { assert(false); }

  virtual GameModelManager &getMgr() override { return M; }

private:
  std::string Port;
  GameModelManager M;
};

SocketSyncClient::SocketSyncClient(const std::string &Host,
                                   const std::string &Port)
    : Port(Port) {
}

void SocketSyncClient::Init() {

}

void SocketSyncClient::PushModel(GameModel M) {

}

GameModel SocketSyncClient::PullModel() {
  return GameModel();
}

std::unique_ptr<ModelSync>
ModelSync::CreateSocketClient(const std::string &Host,
                              const std::string &Port) {
  return std::unique_ptr<ModelSync>(new SocketSyncClient(Host, Port));
}
