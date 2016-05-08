//===-- XBeeSyncServer.cpp - XBee Syncing for GameModel ------------ c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/sync/ModelSync.h"

#include "uwhd/model/GameModel.h"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <xbee.h>

class XBeeSyncServer : public ModelSync, GameModelManager {
public:
  XBeeSyncServer(bool Coordinator);
  ~XBeeSyncServer();

  virtual void Init() override;

  virtual void setMgr(GameModelManager *NewM) {
    assert(!M);
    M = NewM;
    M->registerListener(this);
  }

  virtual GameModelManager &getMgr() override { return *M; }

  virtual void modelChanged(GameModel Model) override;

private:
  bool Coordinator;
  struct xbee *XBee;
  GameModelManager *M;

  static const char *XBeeSerialConsole;
  static const int XBeeBaudRate;
};

const char *XBeeSyncServer::XBeeSerialConsole = "/dev/ttyAMA0";
const int   XBeeSyncServer::XBeeBaudRate      = 9600;

XBeeSyncServer::XBeeSyncServer(bool Coordinator)
  : Coordinator(Coordinator), XBee(nullptr), M(nullptr) {
}

XBeeSyncServer::~XBeeSyncServer() {
  if (XBee)
    xbee_shutdown(XBee);
}

void XBeeSyncServer::Init() {
  if (auto Err = xbee_setup(&XBee, "xbee1", XBeeSerialConsole, XBeeBaudRate)) {
    std::cerr << "error: could not init xbee: "
              << Err << " " << xbee_errorToStr(Err) << "\n";
    abort();
  }
}

void XBeeSyncServer::modelChanged(GameModel Model) {
  assert(XBee && "xbee not inited?");

  // Send to the broadcast address, so everyone can hear it
  struct xbee_conAddress Address;
  memset(&Address, 0, sizeof(Address));
  Address.addr64_enabled = 1;
  Address.addr64[0] = 0x00;
  Address.addr64[1] = 0x00;
  Address.addr64[2] = 0x00;
  Address.addr64[3] = 0x00;
  Address.addr64[4] = 0x00;
  Address.addr64[5] = 0x00;
  Address.addr64[6] = 0xFF;
  Address.addr64[7] = 0xFF;

  struct xbee_con *Con;
  if (auto Err = xbee_conNew(XBee, &Con, "64-bit Data", &Address)) {
    std::cerr << "error: could not open xbee connection: "
              << Err << " " << xbee_errorToStr(Err) << "\n";
    abort();
  }

  // Ignore the ACK
  struct xbee_conSettings Settings;
  xbee_conSettings(Con, nullptr, &Settings);
  Settings.disableAck = 1;
  xbee_conSettings(Con, &Settings, nullptr);

  std::string Message = Model.serialize();
  xbee_conTx(Con, nullptr, Message.c_str(), 1);

  if (auto Err = xbee_conEnd(Con)) {
    std::cerr << "error: could not close the xbee connection: "
              << Err << " " << xbee_errorToStr(Err) << "\n";
    abort();
  }
}

//ModelSync *CreateXBeeSyncServer(bool Coordinator) {
ModelSync *CreateXBeeSync(bool Coordinator) {
  return new XBeeSyncServer(Coordinator);
}

