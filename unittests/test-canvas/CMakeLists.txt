add_executable(test-canvas test-canvas.cpp)
target_link_libraries(test-canvas
    uwhdmodel
    uwhdcanvas
    gtest
    pthread)
