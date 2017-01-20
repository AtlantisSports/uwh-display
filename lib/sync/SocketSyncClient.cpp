//===-- SocketSyncClient.cpp - Socket Syncing for GameModel -------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/sync/ModelSync.h"
#include "uwhd/config/Config.h"

#ifdef UWHD_BUILD_CLIENT

#include "uwhd/model/GameModel.h"

#include <cstring>
#include <string>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory>
#include <cassert>
#include <iostream>

static const int MAX_EVENTS = 16;

class SocketSyncClient : public ModelSync, GameModelManager {
public:
  SocketSyncClient(const std::string &Host, const std::string &Port);

  virtual void Init() override;

  virtual void setMgr(GameModelManager *M) override { assert(false); }

  virtual GameModelManager &getMgr() override { return *this; }

  virtual void modelChanged(GameModel Model) override;

private:
  std::string Host;
  std::string Port;
};

SocketSyncClient::SocketSyncClient(const std::string &Host,
                                   const std::string &Port)
    : Host(Host), Port(Port) {
}

void SocketSyncClient::Init() {
}

void SocketSyncClient::modelChanged(GameModel Model) {
  int SockFD = socket(AF_INET, SOCK_STREAM, 0);

  if (SockFD < 0) {
    perror("Error opening socket");
    exit(1);
  }

  struct hostent *server = gethostbyname(Host.c_str());

  if (server == nullptr) {
    fprintf(stderr, "ERROR, no such host\n");
    exit(0);
  }

  struct sockaddr_in serv_addr;
  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy(server->h_addr, &serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(atoi(Port.c_str()));

  if (connect(SockFD, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
    perror("ERROR connecting");
    exit(1);
  }

  std::string Ser = Model.serialize();
  write(SockFD, Ser.c_str(), Ser.size());

  close(SockFD);
}

#endif // UWHD_BUILD_CLIENT

ModelSync *CreateSocketClient(const std::string &Host, const std::string &Port) {
#ifdef UWHD_BUILD_CLIENT
  return new SocketSyncClient(Host, Port);
#else
  return nullptr;
#endif
}
