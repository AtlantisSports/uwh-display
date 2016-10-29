if(UWHD_BUILD_SERVER)
  list(APPEND UWHD_SYNC_SOURCES SocketSyncServer.cpp)
endif()

if(UWHD_BUILD_CLIENT)
  list(APPEND UWHD_SYNC_SOURCES SocketSyncClient.cpp)
endif()

if(UWHD_HAVE_LIBXBEE3)
  list(APPEND UWHD_SYNC_SOURCES XBeeSyncServer.cpp)
endif()

add_library(uwhdsync ${UWHD_SYNC_SOURCES})

if(UWHD_HAVE_LIBXBEE3)
  target_link_libraries(uwhdsync ${LIBXBEE3})
endif()

target_include_directories(uwhdsync PUBLIC ${CMAKE_SOURCE_DIR}/include)
