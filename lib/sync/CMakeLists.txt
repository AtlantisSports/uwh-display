if(UWHD_BUILD_SERVER AND UWHD_BUILD_CLIENT)
  add_library(uwhdsync
              SocketSyncServer.cpp
              SocketSyncClient.cpp
              XBeeSyncServer.cpp)
elseif(UWHD_BUILD_SERVER)
  add_library(uwhdsync
              SocketSyncServer.cpp
              XBeeSyncServer.cpp)
elseif(UWHD_BUILD_CLIENT)
  add_library(uwhdsync
              SocketSyncClient.cpp
              XBeeSyncServer.cpp)
endif()

target_link_libraries(uwhdsync ${LIBXBEE3})

target_include_directories(uwhdsync PUBLIC ${CMAKE_SOURCE_DIR}/include)
