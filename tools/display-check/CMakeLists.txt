add_executable(display-check display-check.cpp)
target_link_libraries(display-check
    uwhdmodel
    uwhdcanvas
    uwhddisplay)
