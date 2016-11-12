add_executable(test-display test-display.cpp)
target_link_libraries(test-display
    uwhdmodel
    uwhdcanvas
    uwhddisplay
    gtest
    pthread)
