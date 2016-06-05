%module sync
%{
#include "uwhd/sync/ModelSync.h"
%}

%include "std_vector.i"
%include "std_string.i"

%nodefaultctor ModelSync;
%nodefaultctor ModelSyncServer;
%newobject CreateSocketServer;
%newobject CreateSocketClient;
%newobject CreateXBeeSync;

%include "uwhd/sync/ModelSync.h"
