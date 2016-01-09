add_executable(uwhdc uwhdc.cpp)
add_dependencies(uwhdc rgbmatrix)
target_link_libraries(uwhdc
    uwhdsync
    uwhdconsole
    uwhdmodel)
