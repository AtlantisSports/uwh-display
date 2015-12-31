cmake_minimum_required(VERSION 3.0)

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
set(LIBRGBMATRIX "${CMAKE_SOURCE_DIR}/matrix/lib/librgbmatrix.a")
add_custom_command(
    OUTPUT "${LIBRGBMATRIX}"
    COMMAND make -C "${CMAKE_SOURCE_DIR}/matrix")
add_custom_target(rgbmatrix
    DEPENDS "${LIBRGBMATRIX}")
include_directories("${CMAKE_SOURCE_DIR}/matrix/include")
link_directories("${CMAKE_SOURCE_DIR}/matrix/lib")

# -----------------------------------------------------------------------------
# Build libraries, and tools
# -----------------------------------------------------------------------------
add_subdirectory(lib)
add_subdirectory(tools)
