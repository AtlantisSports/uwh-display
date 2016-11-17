add_executable(uwhdd uwhdd.cpp)
add_dependencies(uwhdd rgbmatrix)
target_link_libraries(uwhdd
    uwhdsync
    uwhddisplay
    uwhdmodel
    uwhdcanvas
    ${LIBRGBMATRIX}
    rt
    m
    pthread)
