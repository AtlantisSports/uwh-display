add_executable(uwhdi uwhdi.cpp)
add_dependencies(uwhdi rgbmatrix)
target_link_libraries(uwhdi uwhddisplay uwhdmodel ${LIBRGBMATRIX} rt m pthread)
