add_library(uwhddisplay
            BigNumber.cpp
            CMakeLists.txt
            GameDisplay.cpp
            Status.cpp
            TimeDisplay.cpp)

target_include_directories(uwhddisplay PUBLIC ${CMAKE_SOURCE_DIR}/include)
