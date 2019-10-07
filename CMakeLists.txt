# Works with 3.11 and tested through 3.15
cmake_minimum_required(VERSION 3.1...3.15)


# Project setup
project(seq-parser VERSION 0.1.0
                    DESCRIPTION "Sequence parser"
                    LANGUAGES CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

message("Add external")
add_subdirectory(external EXCLUDE_FROM_ALL)

message("Add src")
add_subdirectory(src)

message("Add apps")
add_subdirectory(apps)

target_link_libraries(seqparser
    PRIVATE # fmt is only needed to build, not to use this library
        fmt::fmt # Use the namespaced version to make sure we have the target and not the static lib only
)
# target_include_directories(seqparser
#     PUBLIC 
#         $<INSTALL_INTERFACE:include>    
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#     PRIVATE
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/seq-parser
# )

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.69 REQUIRED COMPONENTS filesystem)
message(STATUS "Boost version: ${Boost_VERSION}")

# This is needed if your Boost version is newer than your CMake version
# or if you have an old version of CMake (<3.5)
if(NOT TARGET Boost::filesystem)
    add_library(Boost::filesystem IMPORTED INTERFACE)
    set_property(TARGET Boost::filesystem PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
    set_property(TARGET Boost::filesystem PROPERTY
        INTERFACE_LINK_LIBRARIES ${Boost_LIBRARIES})
endif()

target_link_libraries(seqparser PUBLIC Boost::filesystem)