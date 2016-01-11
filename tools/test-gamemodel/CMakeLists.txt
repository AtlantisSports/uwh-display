add_executable(test-gamemodel test-gamemodel.cpp)
target_link_libraries(test-gamemodel
    uwhdmodel
    pthread)
