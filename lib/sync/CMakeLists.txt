if(UWHD_BUILD_SERVER)
  list(APPEND UWHD_SYNC_SOURCES SocketSyncServer.cpp)
endif()

if (UWHD_BUILD_CLIENT)
  list(APPEND UWHD_SYNC_SOURCES SocketSyncClient.cpp)
endif()

list(APPEND UWHD_SYNC_SOURCES XBeeSyncServer.cpp)

add_library(uwhdsync ${UWHD_SYNC_SOURCES})

target_link_libraries(uwhdsync ${LIBXBEE3})

target_include_directories(uwhdsync PUBLIC ${CMAKE_SOURCE_DIR}/include)
