add_library(uwhdsync
            SocketSync.cpp)

target_include_directories(uwhdsync PUBLIC ${CMAKE_SOURCE_DIR}/include)
