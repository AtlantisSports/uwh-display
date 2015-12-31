add_library(uwhddisplay
            BigNumber.cpp
            CMakeLists.txt
            ColorSettings.cpp
            GameDisplay.cpp
            SecondsRing.cpp
            TimeDisplay.cpp)

target_include_directories(uwhddisplay PUBLIC ${CMAKE_SOURCE_DIR}/include)
