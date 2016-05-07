%module sync
%{
#include "uwhd/sync/ModelSync.h"
%}

%include "uwhd/sync/ModelSync.h"

%newobject CreateSocketServer;
%newobject CreateSocketClient;


