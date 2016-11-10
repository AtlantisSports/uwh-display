add_library(uwhdcanvas
            Canvas.cpp
            PPMCanvasViewer.cpp
            LEDCanvasViewer.cpp)

target_include_directories(uwhdcanvas PUBLIC ${CMAKE_SOURCE_DIR}/include)
