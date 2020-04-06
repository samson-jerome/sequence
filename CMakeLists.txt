cmake_minimum_required(VERSION 3.5)

# ---------------------------------------------------------------------
# Project setup
project (sequence VERSION 0.2.0
                    LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)

# Use GNUInstallDirs
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# ---------------------------------------------------------------------
# Add dependencies

# external submodule
add_subdirectory (external EXCLUDE_FROM_ALL)

# ---------------------------------------------------------------------
# Add app and lib sources
add_subdirectory (src)

# ---------------------------------------------------------------------
# Add tests using following Catch2
option (PACKAGE_TESTS "Build the tests" ON)
if (PACKAGE_TESTS)
    enable_testing ()
    add_subdirectory (test)
endif()