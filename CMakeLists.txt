# Works with 3.11 and tested through 3.15
cmake_minimum_required(VERSION 3.1...3.15)


# Project setup
project(seq-parser VERSION 0.1.0
                    DESCRIPTION "Sequence parser"
                    LANGUAGES CXX)


# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)

message("Add external")
add_subdirectory(external EXCLUDE_FROM_ALL)

message("Add src")
add_subdirectory(src)

message("Add apps")
add_subdirectory(apps)

# target_include_directories(seqparser
#     PUBLIC 
#         $<INSTALL_INTERFACE:include>    
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#     PRIVATE
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/seq-parser
# )

