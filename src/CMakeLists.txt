cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Project setup
project(seq-parser VERSION 0.1.0
                    DESCRIPTION "Sequence parser"
                    LANGUAGES CXX)


# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)


# Library
add_library(seqparser seq-parser/src/parser.cpp seq-parser/src/collection.cpp)

target_include_directories(seqparser
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/seq-parser/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/seq-parser/src
)

# Apps
add_executable(lss apps/lss/src/lss.cpp)
target_include_directories(lss
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/seq-parser/include>)

target_link_libraries(lss PUBLIC seqparser)