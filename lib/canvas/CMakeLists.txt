add_library(uwhdcanvas
            Canvas.cpp
            PPMCanvasViewer.cpp
            LEDCanvasViewer.cpp)
if(UWHD_BUILD_DISPLAY)
  add_dependencies(uwhdcanvas rgbmatrix)
endif()

target_include_directories(uwhdcanvas PUBLIC ${CMAKE_SOURCE_DIR}/include)
