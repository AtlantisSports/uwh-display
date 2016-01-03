//===-- SocketSyncClient.cpp - Socket Syncing for GameModel -------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/sync/ModelSync.h"

#include <string>

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

private:
  std::string Port;
};

SocketSyncClient::SocketSyncClient(const std::string &Host,
                                   const std::string &Port)
    : Port(Port) {
}

static int
create_and_bind(const char *port)
{
  struct addrinfo hints;
  struct addrinfo *result, *rp;

  memset (&hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
  hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
  hints.ai_flags = AI_PASSIVE;     /* All interfaces */

  int s = getaddrinfo(NULL, port, &hints, &result);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror (s));
    return -1;
  }

  int SocketFD;
  for (rp = result; rp != NULL; rp = rp->ai_next)
  {
    SocketFD = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (SocketFD == -1)
      continue;

    s = bind(SocketFD, rp->ai_addr, rp->ai_addrlen);
    if (s == 0)
    {
      /* We managed to bind successfully! */
      break;
    }

    close(SocketFD);
  }

  if (rp == NULL)
  {
    fprintf(stderr, "Could not bind\n");
    return -1;
  }

  freeaddrinfo(result);

  return SocketFD;
}

static int
make_socket_non_blocking (int SocketFD)
{
  int flags = fcntl(SocketFD, F_GETFL, 0);
  if (flags == -1) {
    perror("fcntl");
    return -1;
  }

  flags |= O_NONBLOCK;
  int s = fcntl(SocketFD, F_SETFL, flags);
  if (s == -1) {
    perror("fcntl");
    return -1;
  }

  return 0;
}

static void accept_connection(int SocketFD, int EPollFD,
                              struct epoll_event &E) {
  // We have a notification on the listening socket, which
  // means one or more incoming connections.
  while (1) {
    struct sockaddr in_addr;
    socklen_t in_len;
    int infd;
    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

    in_len = sizeof in_addr;
    infd = accept(SocketFD, &in_addr, &in_len);
    if (infd == -1) {
      if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
        // We have processed all incoming connections.
        break;
      } else {
        perror("accept");
        break;
      }
    }

    int s = getnameinfo (&in_addr, in_len,
        hbuf, sizeof hbuf,
        sbuf, sizeof sbuf,
        NI_NUMERICHOST | NI_NUMERICSERV);
    if (s == 0) {
      printf("Accepted connection on descriptor %d "
             "(host=%s, port=%s)\n", infd, hbuf, sbuf);
    }

    // Make the incoming socket non-blocking and add it to the
    // list of fds to monitor.
    s = make_socket_non_blocking(infd);
    if (s == -1)
      abort();

    E.data.fd = infd;
    E.events = EPOLLIN | EPOLLET;
    s = epoll_ctl(EPollFD, EPOLL_CTL_ADD, infd, &E);
    if (s == -1) {
      perror("epoll_ctl");
      abort();
    }
  }
}

static void process_event(int SocketFD, struct epoll_event &E) {
  /* We have data on the fd waiting to be read. Read and
     display it. We must read whatever data is available
     completely, as we are running in edge-triggered mode
     and won't get a notification again for the same
     data. */
  int done = 0;

  while (1) {
    char buf[512];

    ssize_t count = read(E.data.fd, buf, sizeof(buf));
    if (count == -1) {
      /* If errno == EAGAIN, that means we have read all
         data. So go back to the main loop. */
      if (errno != EAGAIN) {
        perror("read");
        done = 1;
      }
      break;
    } else if (count == 0) {
      /* End of file. The remote has closed the
         connection. */
      done = 1;
      break;
    }

    /* Write the buffer to standard output */
    int s = write(STDOUT_FILENO, buf, count);
    if (s == -1) {
      perror("write");
      abort();
    }
  }

  if (done) {
    printf("Closed connection on descriptor %d\n", E.data.fd);

    /* Closing the descriptor will make epoll remove it
       from the set of descriptors which are monitored. */
    close(E.data.fd);
  }
}

void SocketSyncClient::Init() {
  int SocketFD, s;
  struct epoll_event event;
  struct epoll_event *events;

  SocketFD = create_and_bind(Port.c_str());
  if (SocketFD == -1)
    abort();

  s = make_socket_non_blocking(SocketFD);
  if (s == -1)
    abort();

  s = listen(SocketFD, SOMAXCONN);
  if (s == -1) {
    perror("listen");
    abort();
  }

  int EPollFD = epoll_create1(0);
  if (EPollFD == -1) {
    perror("epoll_create");
    abort();
  }

  event.data.fd = SocketFD;
  event.events = EPOLLIN | EPOLLET;
  s = epoll_ctl(EPollFD, EPOLL_CTL_ADD, SocketFD, &event);
  if (s == -1) {
    perror("epoll_ctl");
    abort();
  }

  /* Buffer where events are returned */
  events = (struct epoll_event*)calloc(MAX_EVENTS, sizeof(event));

  /* The event loop */
  while (1) {
    int n = epoll_wait(EPollFD, events, MAX_EVENTS, -1);
    for (int i = 0; i < n; i++)
    {
      if ((events[i].events & EPOLLERR) ||
          (events[i].events & EPOLLHUP) ||
          !(events[i].events & EPOLLIN)) {
        // An error has occured on this fd, or the socket is not
        // ready for reading (why were we notified then?)
        fprintf(stderr, "epoll error\n");
        close(events[i].data.fd);
      } else if (SocketFD == events[i].data.fd)
        accept_connection(SocketFD, EPollFD, event);
      else
        process_event(SocketFD, events[i]);
    }
  }

  free(events);

  close(SocketFD);
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
