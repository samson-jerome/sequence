# Get library version from include/sequence.h and put it in SEQUENCE_VERSION
function(extract_version)
    file(READ "${CMAKE_CURRENT_LIST_DIR}/include/sequence/sequence.h" file_contents)
    string(REGEX MATCH "SEQUENCE_VER_MAJOR ([0-9]+)" _ "${file_contents}")

    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract major version number from sequence.h")
    endif()
    set(ver_major ${CMAKE_MATCH_1})

    string(REGEX MATCH "SEQUENCE_VER_MINOR ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract minor version number from sequence.h")
    endif()

    set(ver_minor ${CMAKE_MATCH_1})
    string(REGEX MATCH "SEQUENCE_VER_PATCH ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract patch version number from sequence.h")
    endif()
    set(ver_patch ${CMAKE_MATCH_1})

    set(SEQUENCE_VERSION_MAJOR ${ver_major} PARENT_SCOPE)
    set(SEQUENCE_VERSION_MINOR ${ver_minor} PARENT_SCOPE)
    set(SEQUENCE_VERSION_PATCH ${ver_patch} PARENT_SCOPE)
    set(SEQUENCE_VERSION "${ver_major}.${ver_minor}.${ver_patch}" PARENT_SCOPE)
endfunction()