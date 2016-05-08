add_executable(xbee-node-detect xbee-node-detect.cpp)
target_link_libraries(xbee-node-detect
    /usr/lib/libxbeep.so
    /usr/lib/libxbee.so
    pthread
    rt)
