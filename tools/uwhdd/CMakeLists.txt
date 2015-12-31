add_executable(uwhdd uwhdd.cpp)
target_link_libraries(uwhdd uwhd ${LIBRGBMATRIX} rt m pthread)
