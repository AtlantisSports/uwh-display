if(UWHD_BUILD_SERVER AND UWHD_BUILD_CLIENT)
  add_library(uwhdsync
              SocketSyncServer.cpp
              SocketSyncClient.cpp)
elseif(UWHD_BUILD_SERVER)
  add_library(uwhdsync
              SocketSyncServer.cpp)
elseif(UWHD_BUILD_CLIENT)
  add_library(uwhdsync
              SocketSyncClient.cpp)
endif()

target_include_directories(uwhdsync PUBLIC ${CMAKE_SOURCE_DIR}/include)
