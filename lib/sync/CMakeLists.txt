add_library(uwhdsync
            SocketSyncServer.cpp
            SocketSyncClient.cpp)

target_include_directories(uwhdsync PUBLIC ${CMAKE_SOURCE_DIR}/include)
