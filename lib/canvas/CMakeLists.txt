if(UWHD_BUILD_DISPLAY)
  set(DISPLAY_SOURCES LEDCanvasViewer.cpp)
else()
  set(DISPLAY_SOURCES NOLEDCanvasViewer.cpp)
endif()

add_library(uwhdcanvas
            Canvas.cpp
            PPMCanvasViewer.cpp
            ${DISPLAY_SOURCES})

target_include_directories(uwhdcanvas PUBLIC ${CMAKE_SOURCE_DIR}/include)
