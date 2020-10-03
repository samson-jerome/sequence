
# Copyright(c) 2020 sequence authors Distributed under the MIT License (http://opensource.org/licenses/MIT)

cmake_minimum_required (VERSION 3.5)


if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    message (STATUS "VCPKG_CMAKE_TOOLCHAIN: ${CMAKE_TOOLCHAIN_FILE}")
endif()

if (NOT WIN32)
    set (CMAKE_CXX_COMPILER clang++)
endif()

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)

# ---------------------------------------------------------------------
# Project setup
# ---------------------------------------------------------------------
set (SEQUENCE_VERSION 0.2.0)
project (sequence VERSION ${SEQUENCE_VERSION} LANGUAGES CXX)
message (STATUS "Build sequence: ${SEQUENCE_VERSION}")

include (GNUInstallDirs)

# ---------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif ()

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# ---------------------------------------------------------------------
# Set SEQUENCE_MASTER_PROJECT to ON if we are building sequence
# ---------------------------------------------------------------------
# Check if sequence is being used directly or via add_subdirectory, but allow overriding
if (NOT DEFINED SEQUENCE_MASTER_PROJECT)
    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set (SEQUENCE_MASTER_PROJECT ON)
    else ()
        set (SEQUENCE_MASTER_PROJECT OFF)
    endif ()
endif ()
message(DEBUG "DBG: SEQUENCE_MASTER_PROJECT = ${SEQUENCE_MASTER_PROJECT}")

# build tests and example
option (SEQUENCE_BUILD_TESTS "Build the tests" ${SEQUENCE_MASTER_PROJECT})
option (SEQUENCE_BUILD_EXAMPLE "Build example" ${SEQUENCE_MASTER_PROJECT})

# build shared option
option (SEQUENCE_BUILD_SHARED "Build shared library" OFF)

# warning options
option (SEQUENCE_BUILD_WARNINGS "Enable compiler warnings" OFF)

# install options
option (SEQUENCE_INSTALL "Generate the install target" ${SEQUENCE_MASTER_PROJECT})
option (SEQUENCE_FMT_EXTERNAL "Use external fmt library instead of bundled" OFF)
option (SEQUENCE_FMT_EXTERNAL_HO "Use external fmt header-only library instead of bundled" OFF)

if(SEQUENCE_FMT_EXTERNAL AND SEQUENCE_FMT_EXTERNAL_HO)
    message(FATAL_ERROR "SEQUENCE_FMT_EXTERNAL and SEQUENCE_FMT_EXTERNAL_HO are mutually exclusive")
endif()

message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})
# ---------------------------------------------------------------------
# Static/Shared library (shared not supported in windows yet)
# ---------------------------------------------------------------------
set (SEQUENCE_SRC src/sequence.cpp src/collection.cpp)
set (SEQUENCE_ALL_HEADERS include/sequence/sequence.h include/sequence/collection.h)

if(NOT SEQUENCE_FMT_EXTERNAL AND NOT SEQUENCE_FMT_EXTERNAL_HO)
    message(DEBUG "TODO add bundled fmt")
    # list(APPEND SEQUENCE_SRC src/fmt.cpp)
endif()

if (SEQUENCE_BUILD_SHARED)
    add_library (sequence SHARED ${SEQUENCE_SRC} ${SEQUENCE_ALL_HEADERS})
    target_compile_definitions (sequence PUBLIC SEQUENCE_SHARED_LIB)

    if (NOT SEQUENCE_FMT_EXTERNAL AND NOT SEQUENCE_FMT_EXTERNAL_HO)
        target_compile_definitions (sequence PRIVATE FMT_EXPORT PUBLIC FMT_SHARED)
    endif()
else ()
    message(DEBUG "Adding sequence as a static library")
    add_library (sequence STATIC ${SEQUENCE_SRC} ${SEQUENCE_ALL_HEADERS})

	if (NOT SEQUENCE_FMT_EXTERNAL AND NOT SEQUENCE_FMT_EXTERNAL_HO)
        target_compile_definitions (sequence PRIVATE FMT_EXPORT PUBLIC FMT_STATIC)
    endif()
endif()

add_library(sequence::sequence ALIAS sequence)

#target_compile_definitions(sequence PUBLIC SEQUENCE_COMPILED_LIB)
target_include_directories(sequence PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
                                        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

set_target_properties(sequence PROPERTIES VERSION ${SEQUENCE_VERSION})

# ---------------------------------------------------------------------
# Use fmt package if using external fmt
# ---------------------------------------------------------------------
if (SEQUENCE_FMT_EXTERNAL)
    message(DEBUG "Use external FMT")

    if (NOT TARGET fmt::fmt)
        message(DEBUG "Find package fmt")
        find_package (fmt REQUIRED)
    endif()

    # Is this still needed?
    # Naughty way to ensure fmt headers are found when using w32 and VCPKG with CMake
    # ref: https://github.com/microsoft/vcpkg/issues/798
    message (STATUS "Is CMAKE_TOOLCHAIN_FILE defined? ${CMAKE_TOOLCHAIN_FILE}")
    if (WIN32 AND DEFINED CMAKE_TOOLCHAIN_FILE)
        message (STATUS "Workaround to find fmt headers on win32 using VCPKG")
        find_path(SYSTEM_INCLUDE_DIR fmt/format.h)
        include_directories(${SYSTEM_INCLUDE_DIR})
    endif()

    target_compile_definitions(sequence PUBLIC SEQUENCE_FMT_EXTERNAL)
    target_link_libraries (sequence PRIVATE fmt::fmt)

    set (PKG_CONFIG_REQUIRES fmt) # add dependency to pkg-config
endif()

# ---------------------------------------------------------------------
# Build binaries
# ---------------------------------------------------------------------
if (SEQUENCE_BUILD_TESTS)
    message (STATUS "Generating tests")
    enable_testing ()
    add_subdirectory (test)
endif()

if (SEQUENCE_BUILD_EXAMPLE)
    message (STATUS "TODO Generating example(s)")
    # add_subdirectory (example)
endif()

# ---------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------
if (SEQUENCE_INSTALL)
    message(STATUS "Generating install")
    set(project_config_in "${CMAKE_CURRENT_LIST_DIR}/cmake/sequenceConfig.cmake.in")
    set(project_config_out "${CMAKE_CURRENT_BINARY_DIR}/sequenceConfig.cmake")

    set(config_targets_file "sequenceConfigConfigTargets.cmake")
    set(version_config_file "${CMAKE_CURRENT_BINARY_DIR}/sequenceConfigVersion.cmake")

    set(export_dest_dir "${CMAKE_INSTALL_LIBDIR}/cmake/sequence")
    set(pkgconfig_install_dir "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
    set(pkg_config "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc")

    message(DEBUG "DBG: project_config_in = ${project_config_in}")
    message(DEBUG "DBG: project_config_out = ${project_config_out}")
    message(DEBUG "DBG: config_targets_file = ${config_targets_file}")
    message(DEBUG "DBG: version_config_file = ${version_config_file}")
    message(DEBUG "DBG: export_dest_dir = ${export_dest_dir}")
    message(DEBUG "DBG: pkgconfig_install_dir = ${pkgconfig_install_dir}")
    message(DEBUG "DBG: pkg_config = ${pkg_config}")

    # ---------------------------------------------------------------------
    # Include files
    # ---------------------------------------------------------------------
    install(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" PATTERN "fmt/bundled" EXCLUDE)
    install(
        TARGETS sequence
        EXPORT sequence
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    # if(NOT SEQUENCE_FMT_EXTERNAL AND NOT SEQUENCE_FMT_EXTERNAL_HO)
    #     install(DIRECTORY include/${PROJECT_NAME}/fmt/bundled/
    #             DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/fmt/bundled/")
    # endif()

    # ---------------------------------------------------------------------
    # Install pkg-config file
    # ---------------------------------------------------------------------
    get_target_property(PKG_CONFIG_DEFINES sequence INTERFACE_COMPILE_DEFINITIONS)
    string(REPLACE ";" " -D" PKG_CONFIG_DEFINES "${PKG_CONFIG_DEFINES}")
    string(CONCAT PKG_CONFIG_DEFINES "-D" "${PKG_CONFIG_DEFINES}")
    configure_file("cmake/${PROJECT_NAME}.pc.in" "${pkg_config}" @ONLY)
    install(FILES "${pkg_config}" DESTINATION "${pkgconfig_install_dir}")

    # ---------------------------------------------------------------------
    # Install CMake config files
    # ---------------------------------------------------------------------
    install(EXPORT sequence DESTINATION ${export_dest_dir} NAMESPACE sequence:: FILE ${config_targets_file})

    include(CMakePackageConfigHelpers)
    configure_file("${project_config_in}" "${project_config_out}" @ONLY)

    write_basic_package_version_file("${version_config_file}" COMPATIBILITY SameMajorVersion)
    install(FILES "${project_config_out}" "${version_config_file}" DESTINATION "${export_dest_dir}")

endif()