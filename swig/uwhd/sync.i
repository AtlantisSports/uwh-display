%module sync
%{
#include "uwhd/sync/ModelSync.h"
%}

%include "std_vector.i"
%include "std_string.i"

%nodefaultctor ModelSync;
%nodefaultctor ModelSyncPeer;
%nodefaultctor ModelSyncServer;
%newobject CreateSocketServer;
%newobject CreateSocketClient;
%newobject CreateXBeeSync;

namespace std {
  %template(ModelSyncPeerVector) vector<ModelSyncPeer*>;
}

%include "uwhd/sync/ModelSync.h"
