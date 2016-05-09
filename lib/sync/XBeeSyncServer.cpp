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
#include <iomanip>
#include <memory>
#include <string>
#include <sstream>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <xbee.h>

namespace {

struct XBeeSyncPeer : public ModelSyncPeer {
  XBeeSyncPeer(std::string Name, xbee_conAddress Address)
    : Name(Name), Address(Address) {}
  ~XBeeSyncPeer() {}
  virtual std::string name() const override { return Name; }
  virtual std::string addr() const override;
  std::string Name;
  struct xbee_conAddress Address;
};

class XBeeSyncServer : public ModelSyncServer, GameModelManager {
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

  virtual std::vector<ModelSyncPeer*> peers() override {
    return synchronousNodeDiscovery();
  }

protected:
  std::vector<ModelSyncPeer*> synchronousNodeDiscovery();

private:
  bool Coordinator;
  struct xbee *XBee;
  GameModelManager *M;

  static const char *XBeeSerialConsole;
  static const int XBeeBaudRate;
};

} // anonymous namespace

std::string XBeeSyncPeer::addr() const {
  if (Address.addr64_enabled == 1) {
    std::stringstream SS;
    SS << "0x"
       << std::setfill('0') << std::setw(2) << std::hex << std::uppercase
       << (int)Address.addr64[0]
       << std::setfill('0') << std::setw(2) << std::hex << std::uppercase
       << (int)Address.addr64[1]
       << std::setfill('0') << std::setw(2) << std::hex << std::uppercase
       << (int)Address.addr64[2]
       << std::setfill('0') << std::setw(2) << std::hex << std::uppercase
       << (int)Address.addr64[3]
       << " 0x"
       << std::setfill('0') << std::setw(2) << std::hex << std::uppercase
       << (int)Address.addr64[4]
       << std::setfill('0') << std::setw(2) << std::hex << std::uppercase
       << (int)Address.addr64[5]
       << std::setfill('0') << std::setw(2) << std::hex << std::uppercase
       << (int)Address.addr64[6]
       << std::setfill('0') << std::setw(2) << std::hex << std::uppercase
       << (int)Address.addr64[7];
    return SS.str();
  }

  return "<nyi>";
}

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

static XBeeSyncPeer broadcastPeer() {
  xbee_conAddress Addr;
  memset(&Addr, 0, sizeof(Addr));
  Addr.addr64_enabled = 1;
  Addr.addr64[0] = 0x00;
  Addr.addr64[1] = 0x00;
  Addr.addr64[2] = 0x00;
  Addr.addr64[3] = 0x00;
  Addr.addr64[4] = 0x00;
  Addr.addr64[5] = 0x00;
  Addr.addr64[6] = 0xFF;
  Addr.addr64[7] = 0xFF;
  return XBeeSyncPeer("<bcast>", Addr);
}

void XBeeSyncServer::modelChanged(GameModel Model) {
  assert(XBee && "xbee not inited?");

  // Send to the broadcast address, so everyone can hear it
  XBeeSyncPeer BCast = broadcastPeer();

  struct xbee_con *Con;
  if (auto Err = xbee_conNew(XBee, &Con, "64-bit Data", &BCast.Address)) {
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

struct NDConfig {
  std::vector<ModelSyncPeer*> Peers;
  sem_t NDComplete;
};

static void nodeDiscoveryCallback(struct xbee *XBee, struct xbee_con *Con,
                                  struct xbee_pkt **Pkt, void **Data) {
  NDConfig *Config = reinterpret_cast<NDConfig*>(*Data);
  assert(Config);
  if (strncasecmp(reinterpret_cast<const char*>((*Pkt)->atCommand), "ND", 2))
    return;

  if ((*Pkt)->dataLen == 0) {
    // scan complete
    sem_post(&Config->NDComplete);
    return;
  }

  if ((*Pkt)->dataLen < 11) {
    // recieved small packet
    return;
  }

  struct xbee_conAddress PeerAddr;
  memset(&PeerAddr, 0, sizeof(PeerAddr));
  PeerAddr.addr64_enabled = 1;
  PeerAddr.addr64[0] = (*Pkt)->data[2];
  PeerAddr.addr64[1] = (*Pkt)->data[3];
  PeerAddr.addr64[2] = (*Pkt)->data[4];
  PeerAddr.addr64[3] = (*Pkt)->data[5];
  PeerAddr.addr64[4] = (*Pkt)->data[6];
  PeerAddr.addr64[5] = (*Pkt)->data[7];
  PeerAddr.addr64[6] = (*Pkt)->data[8];
  PeerAddr.addr64[7] = (*Pkt)->data[9];
  const char *Name = reinterpret_cast<const char*>(&(*Pkt)->data[11]);
  XBeeSyncPeer *Peer = new XBeeSyncPeer(Name, PeerAddr);
  Config->Peers.push_back(Peer);
}

std::vector<ModelSyncPeer*> XBeeSyncServer::synchronousNodeDiscovery() {
  NDConfig Config;

  // Init the semaphore
  if (sem_init(&Config.NDComplete, 0, 0)) {
    std::cerr << "error: could not init semaphore: "
              << errno << " " << strerror(errno) << "\n";
    abort();
  }

  assert(XBee && "XBee not yet inited?");

  // Open the connection for Local AT
  struct xbee_con *Con;
  if (auto Err = xbee_conNew(XBee, &Con, "Local AT", nullptr)) {
    std::cerr << "error: could not open xbee connection: "
              << Err << " " << xbee_errorToStr(Err) << "\n";
    abort();
  }

  if (auto Err = xbee_conDataSet(Con, &Config, nullptr)) {
    std::cerr << "error: could not set data for callback: "
              << Err << " " << xbee_errorToStr(Err) << "\n";
    abort();
  }

  // Register the node discovery callback
  if (auto Err = xbee_conCallbackSet(Con, nodeDiscoveryCallback, nullptr)) {
    std::cerr << "error: could not register xbee callback: "
              << Err << " " << xbee_errorToStr(Err) << "\n";
    abort();
  }

  // Start the node discovery itself
  unsigned char TxRet;
  if (auto Err = xbee_conTx(Con, &TxRet, "ND"))
    if (Err != XBEE_ETIMEOUT) {
      std::cerr << "error: could not send 'ND' command: "
                << Err << " " << xbee_errorToStr(Err) << "\n";
      abort();
    }

  // Wait 30 seconds for peer discovery
  struct timespec T0;
  clock_gettime(CLOCK_REALTIME, &T0);
  T0.tv_sec += 30;
  if (sem_timedwait(&Config.NDComplete, &T0)) {
    if (errno == ETIMEDOUT)
      std::cerr << "error: timeout while waiting for ND command to complete\n";
    else {
      std::cerr << "error: semaphore failed. Sleeping instead\n";
      usleep(30000000);
    }
  }

  // Close the connection to the XBee
  if (auto Err = xbee_conEnd(Con)) {
    std::cerr << "error: could not close the connection with the xbee: "
              << Err << " " << xbee_errorToStr(Err) << "\n";
    abort();
  }

  return Config.Peers;
}

ModelSyncServer *CreateXBeeSync(bool Coordinator) {
  return new XBeeSyncServer(Coordinator);
}

