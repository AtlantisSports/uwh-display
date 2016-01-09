//===-- uwhdisplayd.cpp - Entry-Point for the uwh-display daemon --- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/display/GameDisplay.h"

#include "uwhd/sync/ModelSync.h"

#include <led-matrix.h>
#include <gpio.h>

#include <climits>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

using namespace rgb_matrix;
using namespace uwhtimer;

class SysLog {
public:
  SysLog() {
    openlog("uwhdd", LOG_PID, LOG_DAEMON);
  }
  ~SysLog() {
    closelog();
  }
};

int main(int argc, char *argv[]) {
  if (argc >= 2 && strcmp(argv[1], "--daemon") == 0) {
    // Check whether we have root privelege
    uid_t uid  = getuid();
    uid_t euid = geteuid();
    if (uid<0 || uid!=euid) {
      std::cerr << "Error: must be run as root." << std::endl;
      exit(-1);
    }

    pid_t pid = fork();
    if (pid < 0) {
      std::cerr << "Error: could not fork." << std::endl;
      exit(-1);
    } else if (pid > 0) {
      // We got a good pid. Kill the parent process,
      // creating an orphaned process.
      exit(0);
    }

    // Start the syslog
    static SysLog Log;

    // Change the file node mask
    umask(0);

    // Create a new SID for the child process
    pid_t sid = setsid();
    if (sid < 0) {
      syslog(LOG_ERR, "uwhdd: Could not create SID for child process.");
      exit(-1);
    }

    // Change the working directory
    if (chdir("/") < 0) {
      syslog(LOG_ERR, "uwhdd: Could not chdir to '/'.");
      exit(-1);
    }

    // Close the standard file descriptors: we can't use them anyway.
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
  } else {
    printf("non-daemon mode\n");
  }

  GPIO IO;
  if (!IO.Init()) {
    syslog(LOG_ERR, "uwhdd: Could not init GPIO.");
    exit(-1);
  }

  // Start the display matrix
  auto Matrix = std::unique_ptr<RGBMatrix>(new RGBMatrix(&IO, 32, 3, 1));
  Matrix->SetPWMBits(11);

  // Start the rendering loop
  auto Display = std::unique_ptr<GameDisplay>(new GameDisplay(&*Matrix));
  Display->Start();

  syslog(LOG_INFO, "uwhdd: Display started.");

  auto SyncServer = CreateSocketServer("5555");
  SyncServer->setMgr(&Display->getMgr());
  SyncServer->Init();

  // Enter the daemon loop
  while (true) {
    sleep(1);
  }

  return 0;
}
