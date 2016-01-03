add_executable(uwhdi uwhdi.cpp)
add_dependencies(uwhdi rgbmatrix)
target_link_libraries(uwhdi
    uwhdsync
    uwhddisplay
    uwhdmodel
    ${LIBRGBMATRIX}
    rt
    m
    pthread)
