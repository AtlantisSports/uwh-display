

add_library(uwhdsync
            SocketSyncServer.cpp
            SocketSyncClient.cpp
            XBeeSyncServer.cpp)

if(UWHD_HAVE_LIBXBEE3)
  target_link_libraries(uwhdsync ${LIBXBEE3})
endif()

target_include_directories(uwhdsync PUBLIC ${CMAKE_SOURCE_DIR}/include)
