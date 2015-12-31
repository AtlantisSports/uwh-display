add_library(uwhd
            BigNumber.cpp
            ColorSettings.cpp
            GameDisplay.cpp
            GameModel.cpp
            SecondsRing.cpp
            TimeDisplay.cpp)

target_include_directories(uwhd PUBLIC ${CMAKE_SOURCE_DIR}/include)
