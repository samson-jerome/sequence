cmake_minimum_required(VERSION 3.5)

# ---------------------------------------------------------------------
# Project setup
project (sequence VERSION 0.1.0
                    DESCRIPTION "Sequence parser"
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

# ---------------------------------------------------------------------
# Create targets
target_link_libraries (sequence PUBLIC Boost::filesystem)
target_link_libraries (sequence PRIVATE fmt::fmt)


# # ---------------------------------------------------------------------
# # Add GTest using following methodology
# # https://github.com/google/googletest/blob/master/googletest/README.md

# # Download and unpack googletest at configure time
# configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
# execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
#   RESULT_VARIABLE result
#   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
# if(result)
#   message(FATAL_ERROR "CMake step for googletest failed: ${result}")
# endif()
# execute_process(COMMAND ${CMAKE_COMMAND} --build .
#   RESULT_VARIABLE result
#   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
# if(result)
#   message(FATAL_ERROR "Build step for googletest failed: ${result}")
# endif()

# # Prevent overriding the parent project's compiler/linker
# # settings on Windows
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# # Add googletest directly to our build. This defines
# # the gtest and gtest_main targets.
# add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
#                  ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
#                  EXCLUDE_FROM_ALL)

# # The gtest/gtest_main targets carry header search path
# # dependencies automatically when using CMake 2.8.11 or
# # later. Otherwise we have to add them here ourselves.
# if (CMAKE_VERSION VERSION_LESS 2.8.11)
#   include_directories("${gtest_SOURCE_DIR}/include")
# endif()

# # Now simply link against gtest or gtest_main as needed. Eg
# # add_executable(example example.cpp)
# # target_link_libraries(example gtest_main)
# # add_test(NAME example_test COMMAND example)

# option (PACKAGE_TESTS "Build the tests" ON)
# if (PACKAGE_TESTS)
#     enable_testing ()
#     add_subdirectory (test)
# endif()

# macro(package_add_test TESTNAME)
#     add_executable(${TESTNAME} ${ARGN})
#     target_link_libraries(${TESTNAME} gtest gmock gtest_main)
#     add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
#     set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
# endmacro()

# package_add_test(first first.cpp)