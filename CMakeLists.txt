cmake_minimum_required(VERSION 3.1...3.15)

# Project setup
project(seq-parser VERSION 0.1.0
                    DESCRIPTION "Sequence parser"
                    LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

# Add app and lib sources
add_subdirectory (src)
add_subdirectory (apps)

# Add external submodule
add_subdirectory (external EXCLUDE_FROM_ALL)

# Add external libs
set (Boost_USE_STATIC_LIBS OFF)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_RUNTIME OFF)
find_package (Boost 1.69 REQUIRED COMPONENTS filesystem)
message (STATUS "Boost version: ${Boost_VERSION}")

# Create targets
target_link_libraries (seqparser PUBLIC Boost::filesystem)
target_link_libraries (seqparser PRIVATE fmt::fmt)

enable_testing ()