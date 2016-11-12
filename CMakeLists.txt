cmake_minimum_required(VERSION 2.8)

option(UWHD_BUILD_CLIENT "Build the client" ON)
option(UWHD_BUILD_SERVER "Build the server" ON)
option(UWHD_BUILD_DISPLAY "Build the display driver" ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# -----------------------------------------------------------------------------
# Verify that the compiler supports c++11
# -----------------------------------------------------------------------------
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
endif()

# -----------------------------------------------------------------------------
# Set the output directory structure
# -----------------------------------------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# -----------------------------------------------------------------------------
# Teach CMake how to build librgbmatrix
# -----------------------------------------------------------------------------
if(UWHD_BUILD_DISPLAY)
set(LIBRGBMATRIX "${CMAKE_SOURCE_DIR}/deps/matrix/lib/librgbmatrix.a")
add_custom_command(
    OUTPUT "${LIBRGBMATRIX}"
    COMMAND make -C "${CMAKE_SOURCE_DIR}/deps/matrix")
add_custom_target(rgbmatrix
    DEPENDS "${LIBRGBMATRIX}")
include_directories("${CMAKE_SOURCE_DIR}/deps/matrix/include")
link_directories("${CMAKE_SOURCE_DIR}/deps/matrix/lib")
endif()

# -----------------------------------------------------------------------------
# Teach CMake how to build googletest
# -----------------------------------------------------------------------------

# Download and unpack googletest at configure time
configure_file(${CMAKE_SOURCE_DIR}/deps/googletest/CMakeLists.txt.in
               ${CMAKE_BINARY_DIR}/googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build)

# The gtest/gmock targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include"
                      "${gmock_SOURCE_DIR}/include")
endif()

# -----------------------------------------------------------------------------
# libxbee3
# -----------------------------------------------------------------------------
set(LIBXBEE3  "/usr/lib/libxbee.so")
set(LIBXBEE3P "/usr/lib/libxbeep.so")
if(EXISTS ${LIBXBEE3})
  set(UWHD_HAVE_LIBXBEE3 YES)
endif()

# -----------------------------------------------------------------------------
# Build libraries, and tools
# -----------------------------------------------------------------------------
add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(swig)
add_subdirectory(unittests)

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------
enable_testing()
add_test(test-canvas    ${CMAKE_BINARY_DIR}/bin/test-canvas)
add_test(test-display   ${CMAKE_BINARY_DIR}/bin/test-display)
add_test(test-gamemodel ${CMAKE_BINARY_DIR}/bin/test-gamemodel)
