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
#include <errno.h>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <memory>
#include <queue>
#include <semaphore.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <termios.h>
#include <thread>
#include <time.h>
#include <unistd.h>

// Serial comm stuff copied from: http://stackoverflow.com/a/6947758
int set_interface_attribs(int fd, int speed, int parity) {
  struct termios tty;
  memset (&tty, 0, sizeof tty);
  if (tcgetattr (fd, &tty) != 0)
  {
    printf("error %d from tcgetattr", errno);
    return -1;
  }

  cfsetospeed (&tty, speed);
  cfsetispeed (&tty, speed);

  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
  // disable IGNBRK for mismatched speed tests; otherwise receive break
  // as \000 chars
  tty.c_iflag &= ~IGNBRK;         // disable break processing
  tty.c_lflag = 0;                // no signaling chars, no echo,
  // no canonical processing
  tty.c_oflag = 0;                // no remapping, no delays
  tty.c_cc[VMIN]  = 0;            // read doesn't block
  tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

  tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
  // enable reading
  tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
  tty.c_cflag |= parity;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CRTSCTS;

  if (tcsetattr (fd, TCSANOW, &tty) != 0)
  {
    printf("error %d from tcsetattr", errno);
    return -1;
  }
  return 0;
}

void set_blocking(int fd, int should_block) {
  struct termios tty;
  memset(&tty, 0, sizeof tty);
  if (tcgetattr(fd, &tty) != 0)
  {
    printf("error %d from tggetattr", errno);
    return;
  }

  tty.c_cc[VMIN]  = should_block ? 1 : 0;
  tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

  if (tcsetattr(fd, TCSANOW, &tty) != 0)
    printf("error %d setting term attributes", errno);
}

namespace {

class XBeeSyncServer : public ModelSyncServer, GameModelManager {
public:
  XBeeSyncServer();
  ~XBeeSyncServer();

  virtual void Init() override;

  virtual void setMgr(GameModelManager *NewM) {
    assert(!M);
    M = NewM;
    M->registerListener(this);
  }

  virtual GameModelManager &getMgr() override { return *M; }

  virtual void modelChanged(GameModel Model) override;

  virtual void heartbeat() override;
private:
  int fd;
  GameModelManager *M;

  static const char *XBeeSerialConsole;
  static const int XBeeBaudRate;
};

class XBeeSyncClient : public ModelSync {
public:
  XBeeSyncClient();
  ~XBeeSyncClient();

  virtual void Init() override;

  virtual void setMgr(GameModelManager *NewM) {
    assert(!M);
    M = NewM;
    //M->registerListener(this);
  }

  virtual GameModelManager &getMgr() override { return *M; }

  void receivedModel(GameModel Model);

protected:

private:
  int fd;
  GameModelManager *M;

  static const char *XBeeSerialConsole;
  static const int   XBeeBaudRate;
};

} // anonymous namespace

const char *XBeeSyncServer::XBeeSerialConsole = "/dev/ttyAMA0";
const int   XBeeSyncServer::XBeeBaudRate      = 9600;
const char *XBeeSyncClient::XBeeSerialConsole = "/dev/ttyAMA0";
const int   XBeeSyncClient::XBeeBaudRate      = 9600;

XBeeSyncServer::XBeeSyncServer()
  : fd(0), M(nullptr) {
}

XBeeSyncServer::~XBeeSyncServer() {
}

void XBeeSyncServer::Init() {
  char *portname = "/dev/ttyAMA0";
  fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0) {
    printf("error %d opening %s: %s", errno, portname, strerror (errno));
    return;
  }
  set_interface_attribs(fd, B9600, 0);  // set speed to 9600 bps, 8n1 (no parity)
  set_blocking(fd, 0);                  // set no blocking
}

void XBeeSyncServer::modelChanged(GameModel Model) {
  std::string Message = Model.serialize();
  write(fd, Message.c_str(), Message.size());
}

void XBeeSyncServer::heartbeat() {
  modelChanged(M->getModel());
}

XBeeSyncClient::XBeeSyncClient()
  : fd(0), M(nullptr) {
}

XBeeSyncClient::~XBeeSyncClient() {
}

void XBeeSyncClient::Init() {
  char *portname = "/dev/ttyAMA0";
  fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0) {
    printf("error %d opening %s: %s", errno, portname, strerror (errno));
    return;
  }
  set_interface_attribs(fd, B9600, 0);  // set speed to 9600 bps, 8n1 (no parity)
  set_blocking(fd, 1);                  // set no blocking

  std::thread([this]() {
    std::queue<char> Queue;
    std::stringstream Msg;
    do {
      char buf[128];
      memset(buf, 0, sizeof(buf));
      int n = read(fd, buf, sizeof(buf));

      //printf("read: [%s]\n", buf);

      for (int i = 0; i < n; ++i)
        Queue.push(buf[i]);

      while (!Queue.empty()) {
        char C = Queue.front();
        Msg << C;
        Queue.pop();

        if (C == 'E') {
          GameModel Model;
          if (GameModel::deSerialize(Msg.str(), Model)) {
            printf("failed to deserialize [%s]\n", Msg.str().c_str());
          } else {
            //printf("updating model:\n");
            receivedModel(Model);
          }

          Msg.str("");
          Msg.clear();
        }
      }

      usleep(100);
    } while (true);
  }).detach();
}

void XBeeSyncClient::receivedModel(GameModel Model) {
  Model.setPrevStartTime();
  M->setModel(Model);
}

ModelSyncServer *CreateXBeeSyncServer() {
  return new XBeeSyncServer();
}

ModelSync *CreateXBeeSyncClient() {
  return new XBeeSyncClient();
}

